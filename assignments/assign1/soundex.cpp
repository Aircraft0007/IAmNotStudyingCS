/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <ostream>
#include <fstream>
#include <cctype>
#include <string>
#include <cstring>
#include <numeric>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
/*This function serves as a helper function; It takes in a char letter and returns a int
 * which contains the digit code corresponds to the letter input  */
int digitCode(char c){

    switch (toUpperCase(c)) {
    case 'A': case 'E': case 'I': case 'O': case 'U': case 'H':  case 'W': case 'Y':
        return 0;
    case 'B': case 'F': case 'P': case 'V':
        return 1;
    case 'C': case 'G': case 'J': case 'K': case 'Q': case 'S': case 'X': case 'Z':
        return 2;
    case 'D': case 'T':
        return 3;
    case 'L':
        return 4;
    case 'M': case 'N':
        return 5;
    case 'R':
        return 6;
    default:
        return -1;
    }
}
/* This function takes one string argument 's' which indicates letters string and
 * returns a string which contains the digit codes corresponds to each letter from input string
 * the helper function 'digitCode' is used here in the for loop to convert each char letter into integer*/
string lettersToDigits(string s){
    string result = "";
    for (int i = 0; i < s.length(); ++i) {
        result += integerToChar(digitCode(s[i]));
    }
    return result;
}



/*This function follows up from above. It takes one string argument 's'
 * which indicates integer string and returns a string, only left with a subset of input string
 * which removes the consecutive replicates of the orginal one. */

// Method 1:
string removeReplicates(string s){
    string result;
    vector<char> v_res;
    for (int i = 0; i < s.length(); ++i) {
        if(s[i]!=s[i-1]){
            v_res.push_back(s[i]);
        }
    }
    return accumulate(v_res.begin(),v_res.end(),result);
}


//Method 2: Similar to Method 1, using Vector<string> instead of vector<char> for data storage.
//string removeReplicates(string s){
//    string result;
//    Vector<string> v_res;
//    for (int i = 0; i < s.length(); ++i) {
//        if(s[i]!=s[i-1]){
//            v_res.add(charToString(s[i]));
//        }
//    }
//    return accumulate(v_res.begin(),v_res.end(),result);
//}


// Method 3: 使用了两个for loop 比较低效，if 判断中去掉 s[i]==s[i-1] 则可以实现完全去重，故保留此版本留作参考
//string removeReplicates(string s){
//    string result;
//    vector<char> v_res;
//    v_res.push_back(s.at(0));
//    for (int i = 1; i < s.length(); ++i) {
//        bool notReplicate = true;
//        for (int j = 0; j < v_res.size(); ++j) {
//            if(s[i]==s[i-1]&&s[i]==v_res[j]){
//                notReplicate = false;
//            }
//        }
//        if(notReplicate){
//            v_res.push_back(s[i]);
//        }

//    }
//    return accumulate(v_res.begin(),v_res.end(),result);

//}


// Method 4: input = "222025" output = "2" "0" "2" "5" 感觉后续处理起来不太方便，先留着后面再回来思考
//string removeReplicates(string s){
//    string result;
//    Vector<string> v_res;
//    for (int i = 0; i < s.length(); ++i) {
//        if(s[i]!=s[i-1]){
//            v_res.add(charToString(s[i]));
//        }
//    }
//    result = v_res.toString();
//    return result.substr(1,result.length()-2);
//}



/*This function takes one string argument 's' and returns a string which contains no zero.*/
string removeZeros(string s){
    string result;
    vector<char> v_res;
    for (int i = 0; i < s.length(); ++i) {
        if(s[i]!='0'){
           v_res.push_back(s[i]);
        }
    }
    return accumulate(v_res.begin(),v_res.end(),result);
}

/* This function bring together all the pre-defined functions being listed above;
 * It follows the soundex coding strategy step by step:
 * 1. trim the name 2. convert the name to digits code 3. remove consecutive replicates
 * 4. Replace the first integer character with the capitalized first letter of name
 * 5. Remove any zeros 6. trimming or padding with zeros to make sure the length of final result is exactly 4.
 * 6. Returns the final result as a string
 */
string soundex(string s) {
    string trimmed = removeNonLetters(s);
    string capitalFirstLetter = toUpperCase(charToString(trimmed.at(0)));
    string digitsCode = lettersToDigits(trimmed);
    string result = removeReplicates(digitsCode);
    result = result.replace(0,1,capitalFirstLetter);
    result = removeZeros(result);
    while(result.length()<4){
        result+='0';
    }
    if(result.length()>4){
        result = result.substr(0,4);
    }

    return result;

}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    Vector<string> v_res;
    while(true){

        string name = getLine("please enter your surname: (PRESS ENTER TO EXIT) ");
        if(name==""){
            return;
        }
        string nameCode = soundex(name);
        for (int i = 0; i < databaseNames.size(); ++i) {
            if(soundex(databaseNames.get(i))==nameCode){
                v_res.add(databaseNames.get(i));
            }

        }
        v_res.sort();
        cout << v_res << endl;
        v_res.clear();
    }




}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test names start with empty spaces or punctions or digits or other non-alpha symnols"){
    string s = "9Alice";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result,"Alice");
    EXPECT_EQUAL(removeNonLetters("!Alice"), "Alice");
    EXPECT_EQUAL(removeNonLetters(" Alice"), "Alice");
    EXPECT_EQUAL(removeNonLetters(",Alice"), "Alice");

}
STUDENT_TEST("Test digitCode function : check  that the input letter character is converted correctly"){
    EXPECT_EQUAL(digitCode('A'),0);
    EXPECT_EQUAL(digitCode('a'),0);
    EXPECT_EQUAL(digitCode('B'),1);

}

STUDENT_TEST("Test lettersToDigits function : check that the letters are converted correctly to digits "){
    EXPECT_EQUAL(lettersToDigits("Curie"),"20600");
}

STUDENT_TEST("Test removeReplicates fucntion : insure that all consecutive repliacted digits are removed "){
    EXPECT_EQUAL(removeReplicates("222025"),"2025");
}

STUDENT_TEST("Test removeZeros fucntion : insure that all zeros are removed "){
    EXPECT_EQUAL(removeZeros("0202"),"22");
}
