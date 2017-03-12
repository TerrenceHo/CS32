//
//  eval.cpp
//  HW2
//
//  Created by Kevin on 2/5/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

string removeSpaces(string infix){
    string temp = "";
    for(int i = 0; i < infix.size(); i++){
        if(infix[i] != ' ')
            temp+=infix[i];
    }
    return temp;
}

int evalPrecedence(char c) {
    if(c == '!'){
        return 3;
    } else if (c == '&'){
        return 2;
    } else if (c == '|'){
        return 1;
    } else
        return -1;
}

bool isValid(string infix){
    if(infix[infix.size()-1] == '!' or infix[infix.size()-1] == '|' or infix[infix.size()-1] == '&' or infix[infix.size()-1] == '(')
        return false;
    if(infix[0] == '&' or infix[0] == '|')
        return false;
    
    int numOperands = 0;
    int numOpenP = 0;
    int numCloseP = 0;
    
    for(int i = 0; i < infix.size(); i++){
        if(isalpha(infix[i]))
            return false;
        
        if(isdigit(infix[i])){
            numOperands++;
            if(i != infix.size() - 1 and isdigit(infix[i+1]))
               return false;
            if(i!=infix.size()-1 and infix[i+1] != '&' and infix[i+1] != '|' and infix[i+1] != ')'){
                return false;
            }
        }
        
        if(infix[i] == '('){
            numOpenP++;
            if(i!= infix.size() -1 and !isdigit(infix[i+1]) and infix[i]!='!' and infix[i]!='(' and infix[i]!=
               ')')
                return false;
        }
        
        if(infix[i] == ')'){
            numCloseP++;
        }
        
        if(infix[i] == '|' or infix[i] == '&'){
            if(infix[i-1] != ')' and !isdigit(infix[i-1]))
                return false;
            if(infix[i+1] != '(' and !isdigit(infix[i+1]) and infix[i+1] != '!')
                return false;
        }
        
        if(infix[i] == '!'){
            if(infix[i+1] != '(' and !isdigit(infix[i+1]) and infix[i+1] != '!')//not equal to ( or a number after is false
                return false;
        }
        
        if(infix[i] == '|' or infix[i] == '&')
            if(infix[i-1] != ')' and !isdigit(infix[i-1]))
                return false;
    }
    if(numOperands == 0)
        return false;
    if(numCloseP!=numOpenP)
        return false;
    for(int i = 0; i < infix.size(); i++){
        if(!isdigit(infix[i]) and infix[i] != '!' and infix[i] != '&' and  infix[i] != '|' and infix[i] != '(' and infix[i] != ')')
            return false;
    }
    return true;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result){
    infix = removeSpaces(infix);
    if(!isValid(infix))
        return 1;
    postfix = "";
    stack<char> operatorStack;
    for(int i = 0; i < infix.length(); i++){
        if(isdigit(infix[i]))
            postfix += infix[i];
        else if (infix[i] == '(')
            operatorStack.push(infix[i]);
        else if (infix[i] == ')'){
            while(operatorStack.top() != '('){
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        else if (infix[i] == '&' or infix[i] == '|' or infix[i] == '!'){
            while(!operatorStack.empty() and operatorStack.top() != '('){
                if(evalPrecedence(infix[i]) <= evalPrecedence(operatorStack.top()) ){
                    postfix += operatorStack.top();
                    operatorStack.pop();
                } else
                    break;
            }
            operatorStack.push(infix[i]);
        }
    }
    while(!operatorStack.empty()){
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    stack<bool> operandStack;
    for(int i = 0; i < postfix.length(); i++){
        if(isdigit(postfix[i]))
            operandStack.push(values[postfix[i] - '0']);
        else if(postfix[i] == '!'){
            if(operandStack.top() == true){
                operandStack.pop();
                operandStack.push(false);
            } else {
                operandStack.pop();
                operandStack.push(true);
            }
        }
        else {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand1 = operandStack.top();
            operandStack.pop();
            bool newOperand;
            if(postfix[i] == '|'){
                if(operand2 == true or operand1 == true)
                    newOperand = true;
                else
                    newOperand = false;
            } else if(postfix[i] == '&'){
                if(operand2 == true and operand1 == true)
                    newOperand = true;
                else
                    newOperand = false;
            }
            operandStack.push(newOperand);
            
        }
    }
    result = operandStack.top();
    return 0;
}

//int main()
//{
//    bool ba[10] = {
//        //  0      1      2      3      4      5      6      7      8      9
//        true,  true,  true,  false, false, false, true,  false, true,  false
//    };
//    string pf;
//    bool answer;
//    assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" && answer == true);
//    assert(evaluate("2| 3", ba, pf, answer) == 0 &&  pf == "23|"  &&  answer);
//    assert(evaluate("(3)", ba, pf, answer) == 0 && pf == "3" && !answer);
//    assert(evaluate("2&(3)", ba, pf, answer) == 0 && pf == "23&" && !answer);
//    assert(evaluate("0 & !9", ba, pf, answer) == 0 && pf == "09!&" && answer);
//    assert(evaluate("!(7|8)", ba, pf, answer) == 0 && pf == "78|!" && !answer);
//    assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" && answer);
//    assert(evaluate("6|4&5", ba, pf, answer) == 0 && pf == "645&|" && answer);
//    assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && pf == "1911&|9|!&!!91&9&!|" && answer);
//    assert(evaluate("8|", ba, pf, answer) == 1);
//    assert(evaluate("4 5", ba, pf, answer) == 1);
//    assert(evaluate("01", ba, pf, answer) == 1);
//    assert(evaluate("()", ba, pf, answer) == 1);
//    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
//    assert(evaluate("2(&8)", ba, pf, answer) == 1);
//    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
//    assert(evaluate("", ba, pf, answer) == 1);
//    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 &&  pf == "43!03&&|"  &&  !answer);
//    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
//    ba[2] = false;
//    ba[9] = true;
//    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
//    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
//    cout << "Passed all tests" << endl;
//}
