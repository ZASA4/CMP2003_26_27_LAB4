/************************************************************
 CMP2003 Lab 4 - Array List : AUTOGRADER (teacher only)
 One file. It includes the student's MainProgram.cpp (without
 its example main) and runs 20 small tests worth 100 points.

 Build : g++ -Wall --std=c++17 autograder.cpp -o grader
 Report: ./grader                (all tests, prints total)
 Single: ./grader --test <id>    (exit code 0 = pass)
 List  : ./grader --list
************************************************************/
#define AUTOGRADING
#include "MainProgram.cpp"

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <initializer_list>
#include <csignal>
#include <csetjmp>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

namespace grader {

struct Failure { string msg; };

// ---------- crash / timeout protection ----------
#ifdef _WIN32
typedef jmp_buf GrJmpBuf;
#define GR_SETJMP(b) setjmp(b)
#define GR_LONGJMP(b, v) longjmp(b, v)
#else
typedef sigjmp_buf GrJmpBuf;
#define GR_SETJMP(b) sigsetjmp(b, 1)
#define GR_LONGJMP(b, v) siglongjmp(b, v)
#endif

static GrJmpBuf gJmp;
static volatile sig_atomic_t gSig = 0;
static string gMsg;
static volatile int gPassed = 0;

extern "C" void onSignal(int s) {
    gSig = s;
    GR_LONGJMP(gJmp, 1);
}

// ---------- tiny assertion helpers ----------
template <class T> string str(const T& v) {
    ostringstream os;
    os << v;
    return os.str();
}
template <class T> string str(const vector<T>& v) {
    string s = "{";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) s += ", ";
        s += str(v[i]);
    }
    return s + "}";
}

template <class A, class E>
void checkEq(const A& actual, const E& expected, const char* expr, int line) {
    if (!(actual == expected))
        throw Failure{ string(expr) + "\n          expected: " + str(expected) +
                       "\n          but got : " + str(actual) +
                       "   (line " + to_string(line) + ")" };
}
#define CHECK_EQ(actual, expected) grader::checkEq((actual), (expected), #actual, __LINE__)
#define CHECK(cond) do { if (!(cond)) throw grader::Failure{ string("check failed: ") + #cond + "   (line " + to_string(__LINE__) + ")" }; } while (0)

// ---------- helpers to build / inspect lists ----------
template <class T>
arrayListType<T> make(initializer_list<T> items) {
    arrayListType<T> l((int)items.size() + 10);
    for (const T& x : items) l.insertEnd(x);
    return l;
}
template <class T>
vector<T> toVec(const arrayListType<T>& l) {
    vector<T> v;
    for (int i = 0; i < l.listSize(); i++) {
        T x{};
        l.retrieveAt(i, x);
        v.push_back(x);
    }
    return v;
}
template <class T>
vector<T> sorted(vector<T> v) {
    sort(v.begin(), v.end());
    return v;
}
static vector<int> V(initializer_list<int> a) { return vector<int>(a); }

// ==========================================================
// TESTS
// ==========================================================

// ---------------- Task 1 : removeAt ----------------
static void t_removeAt_size() {
    auto l = make<int>({10, 20, 30, 40});
    l.removeAt(1);
    CHECK_EQ(l.listSize(), 3);
    l.removeAt(0);
    CHECK_EQ(l.listSize(), 2);
}
static void t_removeAt_removed() {
    auto l = make<int>({10, 20, 30, 40});
    l.removeAt(2);
    CHECK_EQ(sorted(toVec(l)), V({10, 20, 40}));
}
static void t_removeAt_swap() {
    // Swap-with-last: the last element (40) takes the place of index 1.
    auto l = make<int>({10, 20, 30, 40});
    l.removeAt(1);
    CHECK_EQ(toVec(l), V({10, 40, 30}));
}
static void t_removeAt_edges() {
    auto a = make<int>({10, 20, 30});
    a.removeAt(2);                                // remove the last element
    CHECK_EQ(toVec(a), V({10, 20}));
    auto b = make<int>({5});
    b.removeAt(0);                                // remove the only element
    CHECK_EQ(b.listSize(), 0);
    CHECK(b.isEmpty());
}
static void t_removeAt_invalid() {
    auto l = make<int>({10, 20, 30});
    l.removeAt(-1);
    l.removeAt(3);
    l.removeAt(100);
    CHECK_EQ(toVec(l), V({10, 20, 30}));
}

// ---------------- Task 2 : removeAll ----------------
static void t_removeAll_basic() {
    auto l = make<int>({5, 7, 3, 7, 9, 7});
    l.removeAll(7);
    CHECK_EQ(l.listSize(), 3);
    CHECK_EQ(sorted(toVec(l)), V({3, 5, 9}));
}
static void t_removeAll_consecutive() {
    // After removeAt(i) a new element sits at i - it must be checked too.
    auto l = make<int>({7, 7, 7, 1, 7, 7, 2, 7});
    l.removeAll(7);
    CHECK_EQ(sorted(toVec(l)), V({1, 2}));
}
static void t_removeAll_none() {
    auto l = make<int>({1, 2, 3});
    l.removeAll(9);
    CHECK_EQ(toVec(l), V({1, 2, 3}));
}
static void t_removeAll_all() {
    auto l = make<int>({4, 4, 4, 4});
    l.removeAll(4);
    CHECK_EQ(l.listSize(), 0);
    CHECK(l.isEmpty());
    l.insertEnd(1);                               // list must still be usable
    CHECK_EQ(toVec(l), V({1}));
}
static void t_removeAll_edges() {
    arrayListType<int> e(5);
    e.removeAll(3);                               // empty list: nothing happens
    CHECK_EQ(e.listSize(), 0);
    auto a = make<int>({1, 2, 7});                // item at the end
    a.removeAll(7);
    CHECK_EQ(sorted(toVec(a)), V({1, 2}));
    auto b = make<int>({7, 1, 2});                // item at the start
    b.removeAll(7);
    CHECK_EQ(sorted(toVec(b)), V({1, 2}));
}

// ---------------- Task 3 : min ----------------
static void t_min_basic() {
    auto l = make<int>({15, 8, 22, 5, 9});
    CHECK_EQ(l.min(), 5);
    CHECK_EQ(toVec(l), V({15, 8, 22, 5, 9}));     // list must not be modified
}
static void t_min_negatives() {
    auto a = make<int>({3, -2, -9, 0, 4});
    CHECK_EQ(a.min(), -9);
    auto b = make<int>({-1, -5, -3});
    CHECK_EQ(b.min(), -5);
}
static void t_min_position() {
    auto one = make<int>({42});
    CHECK_EQ(one.min(), 42);
    auto first = make<int>({1, 5, 9});
    CHECK_EQ(first.min(), 1);
    auto last = make<int>({9, 5, 1});
    CHECK_EQ(last.min(), 1);
    auto dup = make<int>({3, 1, 1, 3});
    CHECK_EQ(dup.min(), 1);
}
static void t_min_types() {
    auto d = make<double>({2.5, -1.5, 3.25});
    CHECK_EQ(d.min(), -1.5);
    auto s = make<string>({"pear", "apple", "zebra"});
    CHECK_EQ(s.min(), string("apple"));
}
static void t_min_empty() {
    arrayListType<int> l(5);
    (void)l.min();                                // must trigger assert()
}

// ---------------- Task 4 : max ----------------
static void t_max_basic() {
    auto l = make<int>({15, 8, 22, 5, 9});
    CHECK_EQ(l.max(), 22);
    CHECK_EQ(toVec(l), V({15, 8, 22, 5, 9}));     // list must not be modified
}
static void t_max_negatives() {
    auto a = make<int>({3, -2, -9, 0, 4});
    CHECK_EQ(a.max(), 4);
    auto b = make<int>({-3, -2, -9});
    CHECK_EQ(b.max(), -2);
}
static void t_max_position() {
    auto one = make<int>({42});
    CHECK_EQ(one.max(), 42);
    auto first = make<int>({9, 5, 1});
    CHECK_EQ(first.max(), 9);
    auto last = make<int>({1, 5, 9});
    CHECK_EQ(last.max(), 9);
    auto dup = make<int>({3, 9, 9, 3});
    CHECK_EQ(dup.max(), 9);
}
static void t_max_types() {
    auto d = make<double>({2.5, -1.5, 3.25});
    CHECK_EQ(d.max(), 3.25);
    auto s = make<string>({"pear", "apple", "zebra"});
    CHECK_EQ(s.max(), string("zebra"));
}
static void t_max_empty() {
    arrayListType<int> l(5);
    (void)l.max();                                // must trigger assert()
}

// ==========================================================
// TEST TABLE  (id, task title, description, points, expects assert, function)
// ==========================================================
struct Test {
    const char* id;
    const char* task;
    const char* desc;
    int points;
    bool expectAbort;
    void (*fn)();
};

static const Test TESTS[] = {
    {"removeAt.size",        "Task 1 - removeAt()",   "list size decreases by one",                    5,  false, t_removeAt_size},
    {"removeAt.removed",     "Task 1 - removeAt()",   "removed item gone, all others kept",            5,  false, t_removeAt_removed},
    {"removeAt.swap",        "Task 1 - removeAt()",   "last element is swapped into the empty slot",   10, false, t_removeAt_swap},
    {"removeAt.edges",       "Task 1 - removeAt()",   "removing the last / the only element",          5,  false, t_removeAt_edges},
    {"removeAt.invalid",     "Task 1 - removeAt()",   "invalid index leaves the list unchanged",       5,  false, t_removeAt_invalid},

    {"removeAll.basic",      "Task 2 - removeAll()",  "removes every occurrence",                      8,  false, t_removeAll_basic},
    {"removeAll.consecutive","Task 2 - removeAll()",  "consecutive / swapped-in duplicates",           8,  false, t_removeAll_consecutive},
    {"removeAll.none",       "Task 2 - removeAll()",  "absent item: list unchanged",                   4,  false, t_removeAll_none},
    {"removeAll.all",        "Task 2 - removeAll()",  "every element equal: list becomes empty",       5,  false, t_removeAll_all},
    {"removeAll.edges",      "Task 2 - removeAll()",  "empty list, item at start / at end",            5,  false, t_removeAll_edges},

    {"min.basic",            "Task 3 - min()",        "smallest of 5 ints, list not modified",         5,  false, t_min_basic},
    {"min.negatives",        "Task 3 - min()",        "negative numbers",                              4,  false, t_min_negatives},
    {"min.position",         "Task 3 - min()",        "one element / first / last / duplicates",       4,  false, t_min_position},
    {"min.types",            "Task 3 - min()",        "works for double and string (templates)",       4,  false, t_min_types},
    {"min.empty",            "Task 3 - min()",        "assert() fires on an empty list",               3,  true,  t_min_empty},

    {"max.basic",            "Task 4 - max()",        "largest of 5 ints, list not modified",          5,  false, t_max_basic},
    {"max.negatives",        "Task 4 - max()",        "negative numbers",                              4,  false, t_max_negatives},
    {"max.position",         "Task 4 - max()",        "one element / first / last / duplicates",       4,  false, t_max_position},
    {"max.types",            "Task 4 - max()",        "works for double and string (templates)",       4,  false, t_max_types},
    {"max.empty",            "Task 4 - max()",        "assert() fires on an empty list",               3,  true,  t_max_empty},
};
static const int NUM_TESTS = (int)(sizeof(TESTS) / sizeof(TESTS[0]));

// ==========================================================
// RUNNER
// ==========================================================
// Runs one test, protected against exceptions, crashes and infinite loops.
// Returns true on pass; otherwise `gMsg` explains why it failed.
static bool runTest(const Test& t) {
    gSig = 0;
    gMsg.clear();
    gPassed = 0;

    signal(SIGSEGV, onSignal);
    signal(SIGABRT, onSignal);
    signal(SIGFPE, onSignal);
#ifndef _WIN32
    signal(SIGALRM, onSignal);
    alarm(3);                                     // 3 second limit per test
#endif

    // hide anything the student's code prints while a test runs
    ostringstream sink;
    streambuf* oldOut = cout.rdbuf(sink.rdbuf());
    streambuf* oldErr = cerr.rdbuf(sink.rdbuf());
#ifndef _WIN32
    // also silence messages written straight to stderr (e.g. by assert())
    int savedErrFd = dup(2);
    int devNull = open("/dev/null", O_WRONLY);
    if (devNull >= 0) { dup2(devNull, 2); close(devNull); }
#endif

    if (GR_SETJMP(gJmp) == 0) {
        try {
            t.fn();
            if (t.expectAbort)
                gMsg = "expected assert() to fail on an empty list, but nothing happened";
            else
                gPassed = 1;
        } catch (const Failure& f) {
            gMsg = f.msg;
        } catch (const exception& e) {
            gMsg = string("exception thrown: ") + e.what();
        } catch (...) {
            gMsg = "unknown exception thrown";
        }
    } else {
        // we got here through a signal handler
        if (gSig == SIGABRT && t.expectAbort)
            gPassed = 1;
        else if (gSig == SIGABRT)
            gMsg = "program aborted (a failed assert() or abort() call)";
        else if (gSig == SIGSEGV)
            gMsg = "crashed (segmentation fault) - check your indices / pointers";
        else if (gSig == SIGFPE)
            gMsg = "crashed (arithmetic error)";
        else
            gMsg = "timed out - infinite loop?";
    }

#ifndef _WIN32
    alarm(0);
#endif
    cout.rdbuf(oldOut);
    cerr.rdbuf(oldErr);
#ifndef _WIN32
    if (savedErrFd >= 0) { dup2(savedErrFd, 2); close(savedErrFd); }
#endif
    return gPassed == 1;
}

} // namespace grader

int main(int argc, char** argv) {
    using namespace grader;

    // ./main --list           -> "id<TAB>points<TAB>description" per test
    if (argc >= 2 && string(argv[1]) == "--list") {
        for (int i = 0; i < NUM_TESTS; i++)
            cout << TESTS[i].id << "\t" << TESTS[i].points << "\t" << TESTS[i].desc << "\n";
        return 0;
    }

    // ./main --test <id>      -> used by the autograder: exit code 0 = pass
    if (argc >= 3 && string(argv[1]) == "--test") {
        for (int i = 0; i < NUM_TESTS; i++) {
            if (string(argv[2]) == TESTS[i].id) {
                bool ok = runTest(TESTS[i]);
                if (ok) {
                    cout << "PASS " << TESTS[i].id << " (" << TESTS[i].points << " pts): " << TESTS[i].desc << endl;
                    return 0;
                }
                cout << "FAIL " << TESTS[i].id << " (0/" << TESTS[i].points << " pts): " << TESTS[i].desc
                     << "\n      " << gMsg << endl;
                return 1;
            }
        }
        cout << "unknown test id: " << argv[2] << endl;
        return 2;
    }

    // ./main                  -> full grade report for the student
    int earned = 0, total = 0;
    string lastTask;
    cout << "\n==================== LAB 4 - Array List : Grade Report ====================\n";
    for (int i = 0; i < NUM_TESTS; i++) {
        const Test& t = TESTS[i];
        if (lastTask != t.task) {
            int groupPts = 0;
            for (int j = 0; j < NUM_TESTS; j++)
                if (string(TESTS[j].task) == t.task) groupPts += TESTS[j].points;
            cout << "\n" << t.task << "   (" << groupPts << " pts)\n";
            lastTask = t.task;
        }
        bool ok = runTest(t);
        total += t.points;
        if (ok) earned += t.points;
        cout << "  [" << (ok ? "PASS" : "FAIL") << "] "
             << setw(2) << (ok ? t.points : 0) << "/" << setw(2) << left << t.points << right
             << "  " << t.desc << "\n";
        if (!ok) cout << "         -> " << gMsg << "\n";
    }
    cout << "\n------------------------------------------------------------------------\n";
    cout << "  TOTAL: " << earned << " / " << total << "\n";
    cout << "========================================================================\n";
    cout << "(This is the same test set the official autograder uses.)\n";
    return 0;
}
