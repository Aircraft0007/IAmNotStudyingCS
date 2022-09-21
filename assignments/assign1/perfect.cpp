/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one argument `stop` and calculates the sum
 * of all proper divisors of sqrt(`stop`) and its corresponding quotient divisors
 * excluding duplicate divisors and 'stop' itself.
 */
long smarterSum(long stop) {
    long total = 0;
    long quotient;
    for (long divisor = 1; divisor <= sqrt(stop); divisor++) {
        if (stop % divisor == 0 && stop!=divisor) {
            total += divisor;
            quotient = stop/divisor;
            // excluding duplicate divisors and 'stop' itself
            if(quotient!= stop && quotient!=divisor){
                total += quotient;
            }
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is prime.
 * A prime number is a positive number greater than 1 who
 * has only two divisors : 1 and itself.
 */
bool isPrime(long n){
    return smarterSum(n)==1 ? true : false;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function applied Euclid Method for searching perfect numbers.
 * It takes one argument 'n', loop through m = 2^k-1 start by k =1 and
 * checks whether m is prime or not and then compute the corresponding perfect number
 * by the formula: NthperftectNum = m(Nthprime)*2^(k(m)-1)
 * until the nth perfect number has found  which is tracked by index. */
long findNthPerfectEuclid(long n) {
    long index = 0;
    long k = 1;
    long perfectNum = 0;
    while(index < n){
        long m = pow(2,k)-1;
        if(isPrime(m)){
            perfectNum = pow(2,k-1)*m;
            index++;
        }
        k++;
    }
    return perfectNum;
}

/* This function does an exhaustive search for perfect numbers.
 * It takes two arguments 'start' and `end`, searching the range 'start' to `end`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsRange(long start, long end){
    for (long num = start; num < end; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << end << endl;
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}
//STUDENT_TEST("find perfect numbers within given range"){

//     TIME_OPERATION(1000, findPerfectsRange(1,1000));
//     TIME_OPERATION(1000, findPerfectsRange(1001,2000));
//     TIME_OPERATION(1000, findPerfectsRange(2001,3000));
//     TIME_OPERATION(1000, findPerfectsRange(3001,4000));
//     TIME_OPERATION(1000, findPerfectsRange(8001,9000));
//}

// TODO: add your student test cases here
//STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes"){
//     TIME_OPERATION(30000, findPerfectsSmarter(30000));
//     TIME_OPERATION(60000, findPerfectsSmarter(60000));
//     TIME_OPERATION(120000, findPerfectsSmarter(120000));
//     TIME_OPERATION(240000, findPerfectsSmarter(240000));
//}
//STUDENT_TEST("Confirm smarterSum of small inputs") {
//    EXPECT_EQUAL(smarterSum(1), 0);
//    EXPECT_EQUAL(smarterSum(9), 4);
//    EXPECT_EQUAL(smarterSum(25), 6);
//    EXPECT_EQUAL(smarterSum(6), 6);
//    EXPECT_EQUAL(smarterSum(28), 28);
//}
//STUDENT_TEST("Confirm 6 and 28 are perfect") {
//    EXPECT(isPerfect(6));
//    EXPECT(isPerfect(28));
//    EXPECT(isPerfectSmarter(6));
//    EXPECT(isPerfectSmarter(28));
//}
//STUDENT_TEST("Confirm 12 and 98765 are not perfect") {
//    EXPECT(!isPerfect(12));
//    EXPECT(!isPerfect(98765));
//    EXPECT(!isPerfectSmarter(12));
//    EXPECT(!isPerfectSmarter(98765));
//}
//STUDENT_TEST("Test oddballs: 0 and 1 are not perfect") {
//    EXPECT(!isPerfect(0));
//    EXPECT(!isPerfect(1));
//    EXPECT(!isPerfectSmarter(0));
//    EXPECT(!isPerfectSmarter(1));
//}

//STUDENT_TEST("Confirm 33550336 is perfect") {
//    EXPECT(isPerfect(33550336));
//    EXPECT(isPerfectSmarter(33550336));
//}
//STUDENT_TEST("Confirm the first 5 Numbers obtained by Euclid Method is perfect") {
//   EXPECT(isPerfect(findNthPerfectEuclid(1)));
//   EXPECT(isPerfect(findNthPerfectEuclid(2)));
//   EXPECT(isPerfect(findNthPerfectEuclid(3)));
//   EXPECT(isPerfect(findNthPerfectEuclid(4)));
//   EXPECT(isPerfect(findNthPerfectEuclid(5)));
//}

//STUDENT_TEST("Test isPrime: 0 and 1 are not prime, 2 is prime") {
//    EXPECT(!isPrime(0));
//    EXPECT(!isPrime(1));
//    EXPECT(isPrime(2));
//}

//STUDENT_TEST("Confirm the 5th PerfectEculid is 33550336") {
//    EXPECT_EQUAL(findNthPerfectEuclid(5),33550336);
//}

