// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <string>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// This function takes one argument : string s which represents a subsection of a text
// The aim of this function is to remove any punctuations on both end of the string and make sure that the obtained result contains
// at least one letter. Otherwise, return an empty string.
// If the cleaned text is not empty , then return it in lowercase.
string cleanToken(string s)
{
    int start = -1;
    int end = -1;

    //edge case: empty string
    if(s.size()==0){
        return s;
    }



    // find out the start and end point of the valid part of the string
    for (int i = 0; i < s.size(); ++i) {
        if(!ispunct(s[i])){
            start = i;

            for (int j = s.size()-1; j >= i; --j) {
               if(!ispunct(s[j])){
                   end = j;
                   break;
               }


            }
            if(end!=-1){
                break;
            }

        }

    }
    //check if we find out valid string i.e., not purely punctuations
    if(start!=-1&&end!=-1){
        string s_clean =  s.substr(start,(end-start)+1);
        // loop to find any letter within the string
        for (char c : s_clean) {
            if(isalpha(c)){
                return toLowerCase(s_clean);
            }
        }
    }
    //if no letter has found in the previous step or the string is full of punctuations, the return an empty string

    return "";
}

// This function takes one argument: string text which represents the whole content of a web page
// The aim of this function is to divide the web page content into sub texts which is splited by empty space.
// Then, clean each sub text by the 'cleanToken' function defined above.
// Note: Add of empty cleaned sub-text is avoided.
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> str_vec;
    string new_string;
    str_vec = stringSplit(text," ");
    for(string substr: str_vec){
        new_string = cleanToken(substr);
        //if the cleaned string is not empty, then add it to tokens set.
        if(new_string.size()!=0){
            tokens.add(new_string);
        }
    }

    return tokens;
}

/* This function takes two arguments : the string argument  dbfile represents the database file we are searching for
 * and the empty map argument Map<string, Set<string>>& index is designed to store the searching results.
 * The aim of this function is to read a dbfile and store all its contents in lines into a Vector<string>.
 * Next, gather tokens from the lines that represents the web page contents (in this case : odd line numeber).
 * Then, add each sub-text and its corresponding website url into index map.
 * The final index map size(i.e, the number of unique words) is returned as result.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{

    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    Set<string> visited_text;
    Set<string> cleaned_text;
    int nPages =0 ;
    for (int i= 0;  i< lines.size(); i+=2) {
        //obtain all the unique sub-texts in a web page
        cleaned_text  =  gatherTokens(lines[i+1]);
        // loop through all sub-texts
        for(string substr : cleaned_text){
            // if the text is not visited from previous web pages, then put this sub-text as a new kep into index map
            // initialize the value set with current website.
            if(!visited_text.contains(substr)){
                index.put(substr,{lines[i]});
                visited_text.add(substr);
            }else{
                // if the sub-text is already visited, then add the website to the exisiting value set correspond to this sub-text key.

                index[substr].add(lines[i]);

            }

        }
        // add page number by 1 after visiting a web page
        nPages++;

    }
    return nPages;
}

/* This function takes two arguments : the referenced index map and a string argument which represents the query sentence.
 * The aim of this function is to find out the matched website urls from the index map follows the instructions of the query sentence.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> keyword_vec;
    string modifiers = "+-";
    string cleaned;
    // if the query sentence is empty, then report an error
    if(query.size()==0){
        error("no keywords" );
    }
    keyword_vec = stringSplit(query," ");
    // check whether the first keyword contains modifier, if so, then report error.
    if(keyword_vec[0].find_first_of(modifiers)==0){
        error("the first keyword could not contain any modifier");
    }
    for(string kw : keyword_vec){
       if(kw.find_first_of(modifiers)==0){
           //if the keyword is composed only of modifer, then report an error
           if(kw.size()<=1){
               error("invalid keyword");
           }
           string kw_new = kw.substr(1);
           cleaned = cleanToken(kw_new);
           // if the keyword is empty after cleanToken, then report an error
           if(cleaned.size()==0){
               error("empty keyword after cleaning");
           }
           if(kw[0]=='+'){
               result.intersect(index[cleaned]);
           }else if(kw[0]=='-'){
               result.difference(index[cleaned]);
           }

       }else{
           cleaned = cleanToken(kw);
           if(cleaned.size()==0){
               error("empty keyword after cleaning");
           }
           result.unionWith(index[cleaned]);
       }




    }

    return result;
}

// This function combines all the functions defined above to build a searching engine.
// The main function of this searching engine is to ask the user to enter a query sentence and display the query results.
void searchEngine(string dbfile)
{
    //declare a result set
    Set<string> result;
    // contruct a buildIndex and count the number of unique words in the index.
    Map<string, Set<string>> index;
    // obtain the number of web pages in the file
    int nPages = buildIndex(dbfile, index);

    cout<<"Stand by while building index..."<<endl;
    cout<<"Indexed "<< nPages << " containing "<< index.size()<< " unique terms" <<endl<<endl;
    while(true){
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if(query==""){
            return;
        }
        //obtained the matched web page urls and print them out.
        result =findQueryMatches(index,query);
        cout<<"Found "<< result.size() <<" matching pages"<<endl;
        cout<<result.toString()<<endl;

    }

}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


STUDENT_TEST("clear Token on string full of punctuations or on string without letter element "
             "or on string with punctuaction(s) in middle ") {
    EXPECT_EQUAL(cleanToken("/-!-/"), "");
    EXPECT_EQUAL(cleanToken("/12345/"), "");
    EXPECT_EQUAL(cleanToken("/section-10!"), "section-10");
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {};
    EXPECT_EQUAL(gatherTokens("- 123 /1!2-"), expected);
}

STUDENT_TEST("Check operation time for buildIndex function ") {
    Map<string, Set<string>> index;

    TIME_OPERATION(4,buildIndex("res/tiny.txt", index));
    TIME_OPERATION(32,buildIndex("res/website.txt", index));
}
STUDENT_TEST("buildIndex from website.txt, 32 pages, contains token 'lair' ") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);

    EXPECT_EQUAL(nPages, 32);
    EXPECT(index.containsKey("lair"));
}

STUDENT_TEST("buildIndex from tiny.txt, 4 pages, the token 'fish' appears in 3 out of the total 4 web pages ") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(index["fish"].size(),3);
    EXPECT_EQUAL(nPages, 4);
    cout<<index["fish"].toString()<<endl;
}

STUDENT_TEST("Error Test for findQueryMatches from tiny.txt") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    EXPECT_ERROR(findQueryMatches(index, "red +"));
    EXPECT_ERROR(findQueryMatches(index, "+red fish"));
    EXPECT_ERROR(findQueryMatches(index, ""));
    EXPECT_ERROR(findQueryMatches(index, "red /-!-/"));
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries plus capitalized letter involved") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "Fish -reD gReen +blUe ");
    EXPECT_EQUAL(matchesRedOrFish.size(), 1);
}

