/**
 * 词梯（Word Ladder）是刘易斯·卡罗尔发明的一种单词游戏。
 * 游戏会给出一个开始单词和结束单词。玩家需要更改开始单词中的某个字母，
 * 获得一个新词，计作一步。然后玩家需要再次更改所得新词中的某个字母，
 * 获得另一个新词。最终获得结束单词，游戏结束。
 */

#include "main.h"
#include <fstream>
#include <iostream>
#include "vector.h"
using namespace std;

/**
 * @brief findWordLadder - 实现一个单词阶梯查找算法
 *
 * 例如：开始单词 cold 和结束单词 warm 可以得到如下结果。
 *
 * COLD -> CORD -> CARD -> WARD -> WARM
 *
 * @param start
 * @param end
 * @return 将计算出的单词阶梯以栈的形式保存
 */
//Queue< Stack<string> > findWordLadder(string start, string end) {
//    // 创建一个队列用用来储存所有的Wordladders
//    Queue< Stack<string> > allPaths;
//    Queue< Stack<string> > allLadders;
//    Set<string> visited;
//   //初始化 initalLadder, 把start letter 放进去
//    Stack<string> initalLadder = {start};
//    // 将initialLadder 放入队列
//    allLadders.enqueue(initalLadder);
//    //当队列不为空，就一直循环查找队列中储存的Wordladders，
//    //直到发现队列中某个Wordladder的尾部是我们要找的 end letter，则返回该Wordladder。
//    while(!allLadders.isEmpty()){
//        visited.clear();
//        // 按加入顺序取出WordLadder
//        Stack<string> currentLadder = allLadders.dequeue();
//        //找出当前Wordladder的词尾,判断其是否为我们要找的end letter
//        // 如果是，则返回该Wordladder，退出查找 (i.e.,while循环)
//        string currentWord = currentLadder.peek();
//        if(currentWord==end){
//            allPaths.enqueue(currentLadder);
//            continue;
//        }
//        //将currentLadder中的所有单词添加到visited中，防止走回头路。
//        visited += visitedWords(currentLadder);

//        //如果不是end letter，则继续查找下一个单词
//        Set<string> neighboringWords = nextWords(currentWord);
//        //添加所有不存在于已有Wordladder中的neighboring words
//        for ( const string &word: neighboringWords){

//            //create a copy of currentLadder
//            Stack<string> newLadder = currentLadder;

//            cout<<"visited"<<visited.toString()<<endl;
//            cout<<"word"<<word<<endl;
//            cout<<"before"<<newLadder.toString()<<endl;

//            if(!visited.contains(word)){

//                //添加新的neighboring word到currentladder得到一个newLadder
//                 newLadder.push(word);
//                //将newLadder加入队列
//                 allLadders.enqueue(newLadder);

//                 //避免加入end letter
//                 if(word!=end){
//                     visited.add(word);
//                 }

//            }
//            cout<<"after"<<newLadder.toString()<<endl;
//        }

//    }


//    return allPaths;
//}
void removeReplicatePaths(Vector<Stack<string>> &allPaths,Vector<Stack<string>> &finalPaths){

    finalPaths.add(allPaths.get(0));
    for (int i = 1; i < allPaths.size(); ++i) {
         bool isFinal = true;
        for(Stack<string> path:finalPaths){


            Stack<string> wordLadder2 = allPaths.get(i);

            Set<string> visited1 = visitedWords(path);
            Set<string> visited2 = visitedWords(wordLadder2);
            Set<string> intersection = visited1 * visited2;
            if(intersection.size()>2){
               isFinal = false;

            }
        }
        if(isFinal){
            finalPaths.add(allPaths.get(i));
        }
    }
}
Vector< Stack<string> > findWordLadder(string start, string end) {
    // 创建一个队列用用来储存所有的Wordladders
    Vector< Stack<string> > allPaths;
    Queue< Stack<string> > allLadders;
    Set<string> visited;
   //初始化 initalLadder, 把start letter 放进去
    Stack<string> initalLadder = {start};
    // 将initialLadder 放入队列
    allLadders.enqueue(initalLadder);
    //当队列不为空，就一直循环查找队列中储存的Wordladders，
    //直到发现队列中某个Wordladder的尾部是我们要找的 end letter，则返回该Wordladder。
    while(!allLadders.isEmpty()){
//        清空visited，如果执行完整去重操作，则需要注释掉这一行
        visited.clear();
        // 按加入顺序取出WordLadder
        Stack<string> currentLadder = allLadders.dequeue();
        //找出当前Wordladder的词尾,判断其是否为我们要找的end letter
        // 如果是，则返回该Wordladder，退出查找 (i.e.,while循环)
        string currentWord = currentLadder.peek();
        if(currentWord==end){
            allPaths.add(currentLadder);
            continue;
        }
        //将currentLadder中的所有单词添加到visited中，防止走回头路。
        visited += visitedWords(currentLadder);

        //如果不是end letter，则继续查找下一个单词
        Set<string> neighboringWords = nextWords(currentWord);
        //添加所有不存在于已有Wordladder中的neighboring words
        for ( const string &word: neighboringWords){

            //create a copy of currentLadder
            Stack<string> newLadder = currentLadder;

//            cout<<"visited"<<visited.toString()<<endl;
//            cout<<"word"<<word<<endl;
//            cout<<"before"<<newLadder.toString()<<endl;

            if(!visited.contains(word)){

                //添加新的neighboring word到currentladder得到一个newLadder
                 newLadder.push(word);
                //将newLadder加入队列
                 allLadders.enqueue(newLadder);

                 //避免加入end letter，实现完整去重操作需要加入这一行
//                 if(word!=end){
//                     visited.add(word);
//                 }

            }
//            cout<<"after"<<newLadder.toString()<<endl;
        }

    }
//如果需要排除行进路线上的重复单词(不包括start和end letter)，则另外加入removeReplicatePaths方法
//    Vector< Stack<string> > finalPaths;
//    removeReplicatePaths(allPaths,finalPaths);
//    return finalPaths;





    return allPaths;
}

// ******************* 无需修改以下代码 **********************

// ******************* 无需修改以下代码 **********************

int main() {
    // 初始化单词表
    ifstream fin("res/SelectedWords.txt");
    string word;
    while (fin >> word) {
        dictionary.add(word);
    }

    // 程序交互部分
    while (true) {
        string start = getLine("输入开始单词: ");
        if (!start.size())
            break;
        string end = getLine("输入结束单词: ");
        if (!end.size())
            break;
        Vector< Stack<string> > allPaths = findWordLadder(toLowerCase(start), toLowerCase(end));
        for(Stack<string> wordLadder : allPaths){


            if (wordLadder.isEmpty()) {
                cout << "未找到单词阶梯！" << endl;
                continue;
            }



//            Stack<string> wordSet;
//            while (!wordLadder.isEmpty()) {
//               wordSet.push(wordLadder.pop());
//            }
//            cout<< wordSet.toString()<<endl;
            cout<<wordLadder.toString()<<endl;


//            cout << endl;
        }




    }

    cout << "未获取输入，游戏结束！" << endl;
    return 0;
}

/**
 * @brief visitedWords - 由于栈无法遍历查询，转换为集合类型
 * @param currentladder
 * @return 以集合的形式返回栈中包含的所有单词
 */
Set<string> visitedWords(Stack<string> currentLadder) {
    Set<string> visited;
    while (!currentLadder.isEmpty()) {
        visited.add(currentLadder.pop());
    }
    return visited;
}

/**
 * @brief nextWords - 根据提供的 currentWord 找出与其仅一字之差的单词。
 *
 * 例如：cat 对应的单词有 bat、eat、cot、cab、car 等等。
 *
 * @param currentWord
 * @return 将所有找出的单词以集合的形式返回
 */
Set<string> nextWords(string currentWord) {
    Set<string> neighboringWords;
    for (int i = 0; i < currentWord.length(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            string newWord = currentWord;
            newWord[i] = ch;
            if (dictionary.contains(newWord)) {
                neighboringWords.add(newWord);
            }
        }
    }
    return neighboringWords;
}
