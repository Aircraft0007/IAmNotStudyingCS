#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "perfect.h"
#include "soundex.h"
using namespace std;
bool isPrime(long n);
int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }


//findPerfectsRange(1,1000);
//findPerfectsRange(1001,2000);
//    findPerfects(20000);
//findPerfectsSmarter(20000);
    // Comment out the above line and uncomment below line 
    // to switch between running perfect.cpp and soundex.cpp
//    soundexSearch("res/surnames.txt");
//    cout<<findNthPerfectEuclid(5)<<endl;
//    string s = "0202";
//    s.erase(0,1);

//    soundexSearch("C:\\CS_Study\\cs106b\\assignments\\assign1-starter\\res\\surnames.txt");
//    cout<< soundex("Angelou")<<endl;
    cout << endl << "main() completed." << endl;
    return 0;
}


// Do not remove or edit below this line. It is here to to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    long n = 0;
    bool b;
    string s;

    n = divisorSum(n);
    b = isPerfect(n);
    findPerfects(n);

    n = smarterSum(n);
    b = isPerfectSmarter(n);
    findPerfectsSmarter(n);

    n = findNthPerfectEuclid(n);

    s = removeNonLetters(s);
    s = soundex(s);
    soundexSearch(s);
}
