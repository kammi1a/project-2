#include <bits/stdc++.h>
using namespace std;

vector<string> Tokenize(string &expression, unordered_map<string, int> &opMap)
{
    vector<string> tokens = {};
    string temp = "";
    bool lastTokenWasOp = true; // Переменная для отслеживания того, является ли последний токен оператором

    for (char c : expression)
    {
        if (c == ' ')
            continue;
        else if (opMap.find(string(1, c)) != opMap.end())
        {
            if (c == '-' && lastTokenWasOp) // Проверяем, является ли минус унарным
                temp += c;                   // Если да, добавляем его к текущему токену
            else
            {
                if (!temp.empty()) // Если текущий токен не пустой, добавляем его в вектор токенов
                    tokens.push_back(temp);
                temp = ""; // Сбрасываем временную переменную
                tokens.push_back(string(1, c));
                lastTokenWasOp = true; // Устанавливаем флаг оператора для следующего токена
            }
        }
        else
        {
            temp += c;              // Добавляем символ к текущему токену
            lastTokenWasOp = false; // Устанавливаем флаг оператора на false, так как токен не является оператором
        }
    }

    if (!temp.empty())
        tokens.push_back(temp);

    return tokens;
}


vector<string> InfixToPostfix(vector<string> &infix, unordered_map<string, int> &opMap)
{
    vector<string> postfix = {};
    vector<string> stack = {"("};
    infix.push_back(")");
    for (string token : infix)
    {
        if (opMap.find(token) == opMap.end()) // oparand
            postfix.push_back(token);
        else if (token == "(")
            stack.push_back(token);
        else if (token == ")")
        {
            while (stack.back() != "(")
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
        else // operator
        {
            while (stack.size() > 0 && opMap[token] <= opMap[stack.back()])
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        }
    }

    return postfix;
}

float CalculatePostfix(vector<string> &postfix, unordered_map<string, int> &opMap)
{
    vector<float> evaluationStack = {};
    for (string token : postfix)
    {
        if (opMap.find(token) != opMap.end())
        {
            if (token == "s" || token == "r" || token == "a")
            {
                if (evaluationStack.empty())
                {
                    cerr << "Not enough operands for operator " << token << endl;
                    return 0;
                }
                float n1 = evaluationStack.back();
                evaluationStack.pop_back();
                float result;
                switch (token[0])
                {
                case 's':
                    result = sqrt(n1);
                    break;
                case 'r':
                    result = round(n1);
                    break;
                case 'a':
                    result = abs(n1);
                    break;
                }
                evaluationStack.push_back(result);
            }
            else
            {
                if (evaluationStack.size() < 2)
                {
                    cerr << "Not enough operands for operator " << token << endl;
                    return 0;
                }
                float n1 = evaluationStack.back();
                evaluationStack.pop_back();
                float n2 = evaluationStack.back();
                evaluationStack.pop_back();
                float result;
                switch (token[0])
                {
                case '+':
                    result = n2 + n1;
                    break;
                case '-':
                    if (token.size() == 1) // Бинарный оператор -
                        result = n2 - n1;
                    else // Унарный оператор -
                        result = -n1;
                    break;
                case '*':
                    result = n2 * n1;
                    break;
                case '/':
                    result = (n1 != 0) ? n2 / n1 : 0;
                    break;
                case '^':
                    result = pow(n2, n1);
                    break;
                }
                evaluationStack.push_back(result);
            }
        }
        else
        {
            evaluationStack.push_back(stof(token));
        }
    }
    if (evaluationStack.size() != 1)
    {
        cerr << "Invalid expression" << endl;
        return 0;
    }
    return evaluationStack[0];
}

int main()
{
    unordered_map<string, int> opMap = {
        {"^", 4},
        {"*", 3}, {"/", 3},
        {"+", 2}, {"-", 2},
        {"(", 1}, {")", 1},
        {"s", 5}, {"r", 5}, {"a", 5} 
    };

    char choice;
    cout << "Welcome to the Calculator!" << endl << endl;
    do
    {
        string expression;
        cout <<"Please enter your arithmetic expression: ";
        getline(cin, expression);

        vector<string> tokens = Tokenize(expression, opMap);
        tokens = InfixToPostfix(tokens, opMap);
        float result = CalculatePostfix(tokens, opMap);
        cout <<"\nResult: " << result << endl << endl;

        cout << "Do you want to continue? (y/n): ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

    } while (choice == 'y' || choice == 'Y');

    cout << "\nThank you for using the Calculator!" << endl;
    return 0;
}

