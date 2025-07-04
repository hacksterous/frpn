// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_PROGRAM_H_
#define SRC_PROGRAM_H_

// std c++
#include <deque>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

// internal includes
#include "lexer.h"
#include "object.h"
#include "stack.h"

using namespace std;
//< program class: the class containing a string parser, all the programs keywords, a stack for running the program
class Program : public deque<Object*>, public Lexer, public Object {
 public:
    Program(rpnstack& stack__, heap& heap__, Program* parent__ = nullptr)
        : Object(kProgram), stack_(stack__), heap_(heap__), parent_(parent__) {}
    Program(const string& value__, rpnstack& stack__, heap& heap__, Program* parent__ = nullptr)
        : Object(kProgram), value(value__), stack_(stack__), heap_(heap__), parent_(parent__) {}

    virtual ~Program() {
        local_heap_.clear();
        for (Object* obj : *this) delete obj;
        clear();
    }

    virtual Object* Clone() {
        Program* prog = new Program(value, stack_, heap_, parent_);
        prog->value = value;
        for (auto& obj : *this) prog->push_back(obj->Clone());
        return prog;
    }
    virtual string Name() { return string("program"); }
    virtual ostream& Show(ostream& out) { return out << "« " << value << " »"; }

    // parser
    RetValue Parse(const string& entry);

    // running
    RetValue Run();
    RetValue Preprocess();

    RetValue ShowError();
    RetValue ShowError(RetValue err, string& context);
    RetValue ShowError(RetValue err, const char* context);
    void ShowSyntaxError(const char* context);
    RetValue GetLastError(void);

    void ShowStack(bool show_separator = true);

    string value;

    static void ApplyDefault();
    static void Welcome();

    static vector<string>& GetAutocompletionWords();

 private:
    // current error and its context
    RetValue err_;
    string err_context_;

    // global stack holding results for user
    rpnstack& stack_;

    // global heap (sto, rcl)
    heap& heap_;

    // local heap for local loop variables (for..next)
    heap local_heap_;

    // parent prog for inheriting heaps
    Program* parent_;

    // keywords
    struct keyword_t {
        ObjectType type;
        string name;
        object_cb_t fn;
        string comment;
    };
    static vector<keyword_t> keywords_;

    // keywords implementation
    ////

    // branch
    size_t RpnIf(Branch& myobj);
    size_t RpnThen(Branch& myobj);
    size_t RpnElse(Branch& myobj);
    size_t RpnEnd(Branch& myobj);
    size_t RpnDo(Branch& myobj);
    size_t RpnUntil(Branch& myobj);
    void RpnIft(void);
    void RpnIfte(void);
    size_t RpnWhile(Branch& myobj);
    size_t RpnRepeat(Branch& myobj);
    size_t RpnStart(Branch& myobj);
    size_t RpnFor(Branch& myobj);
    size_t RpnNext(Branch& myobj);
    size_t RpnStep(Branch& myobj);
    enum { kStepOut = static_cast<size_t>(-1), kRtError = static_cast<size_t>(-2) };

    // complex
    void RpnReal();
    void RpnImag();
    void RpnArg();
    void RpnConj();
    void RpnR2c();
    void RpnC2r();
    void RpnR2p();
    void RpnP2r();

    // general
    void RpnQuit();
    void RpnHelp();
    void RpnStd();
    void RpnFix();
    void RpnSci();
    void RpnVersion();
    void RpnUname();
    void RpnHistory();
    void RpnType();
    void RpnDefault();
    void RpnPrecision();
    void RpnRound();

    // logs
    void RpnE(void);
    void RpnLog10();
    void RpnAlog10();
    void RpnLog2();
    void RpnAlog2();
    void RpnLn();
    void RpnExp();
    void RpnExpm();
    void RpnLnp1();
    void RpnSinh();
    void RpnAsinh();
    void RpnCosh();
    void RpnAcosh();
    void RpnTanh();
    void RpnAtanh();

    // program
    bool FindVariable(string& variable, Object*& obj);
    void RpnEval(void);
    size_t RpnInprog(Branch& inprog_obj);

    // real
    void RpnPlus();
    void RpnMinus();
    void RpnMul();
    void RpnDiv();
    void RpnNeg();
    void RpnInv();
    void RpnPurcent();
    void RpnPurcentCH();
    void RpnPower();
    void RpnSquareroot();
    void RpnSquare();
    void RpnModulo();
    void RpnAbs();
    void RpnHex();
    void RpnBin();
    void RpnDec();
    void RpnBase();
    void RpnFact();
    void RpnSign();
    void RpnMant();
    void RpnXpon();
    void RpnFloor();
    void RpnCeil();
    void RpnFp();
    void RpnIp();
    void RpnMin();
    void RpnMax();

    // stack
    void RpnSwap(void);
    void RpnDrop(void);
    void RpnDrop2(void);
    void RpnDropn(void);
    void RpnErase(void);
    void RpnDup(void);
    void RpnDup2(void);
    void RpnDupn(void);
    void RpnPick(void);
    void RpnRot(void);
    void RpnDepth(void);
    void RpnRoll(void);
    void RpnRolld(void);
    void RpnOver(void);

    // store
    void RpnSto(void);
    void RpnStoadd(void);
    void RpnStosub(void);
    void RpnStomul(void);
    void RpnStodiv(void);
    void RpnStoneg(void);
    void RpnStoinv(void);
    void RpnRcl(void);
    void RpnEdit(void);
    void AutoRcl(Symbol* symb);
    void RpnPurge(void);
    void RpnVars(void);
    void RpnClusr(void);

    // string
    void RpnInstr();
    void RpnStrout();
    void RpnChr();
    void RpnNum();
    void RpnStrsize();
    void RpnStrpos();
    void RpnStrsub();
    void RpnEndl();

    // test-core
    void RpnTest();
    void RunTestFile(string test_filename, int& total_tests, int& total_tests_failed, int& total_steps,
                     int& total_steps_failed);

    // test
    void RpnSup(void);
    void RpnSupEq(void);
    void RpnInf(void);
    void RpnInfEq(void);
    void RpnDiff(void);
    void RpnEq(void);
    void RpnTestAnd(void);
    void RpnTestOr(void);
    void RpnTestXor(void);
    void RpnTestNot(void);
    void RpnSame(void);

    // trig
    void RpnPi(void);
    void RpnD2r(void);
    void RpnR2d(void);
    void RpnSin(void);
    void RpnAsin(void);
    void RpnCos(void);
    void RpnAcos(void);
    void RpnTan(void);
    void RpnAtan(void);

    // time
    void RpnTime();
    void RpnDate();
    void RpnTicks();
};

// convenience macros for RpnXxx functions
// carefull : some of these macros modify program flow

#define ERROR_CONTEXT(err)           \
    do {                             \
        err_ = (err);                \
        err_context_ = __FUNCTION__; \
    } while (0)

#define MIN_ARGUMENTS(num)                  \
    do {                                    \
        if (stack_.size() < (num)) {        \
            ERROR_CONTEXT(kMissingOperand); \
            return;                         \
        }                                   \
    } while (0)

#define MIN_ARGUMENTS_RET(num, ret)         \
    do {                                    \
        if (stack_.size() < (num)) {        \
            ERROR_CONTEXT(kMissingOperand); \
            return (ret);                   \
        }                                   \
    } while (0)

#define ARG_MUST_BE_OF_TYPE(num, typ)         \
    do {                                       \
        if (stack_.at(num)->type != (typ)) { \
            ERROR_CONTEXT(kBadOperandType);    \
            return;                            \
        }                                      \
    } while (0)

#define ARG_MUST_BE_OF_TYPE_RET(num, typ, ret) \
    do {                                        \
        if (stack_.at(num)->type != (typ)) {  \
            ERROR_CONTEXT(kBadOperandType);     \
            return (ret);                       \
        }                                       \
    } while (0)

#endif  // SRC_PROGRAM_H_
