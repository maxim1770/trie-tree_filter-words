#include <iostream>
#include <string>
#include<Windows.h>
#include <vector>
using namespace std;
#define ALPHABET_SIZE 33
#define ROOT_LETTER '\0'

using namespace std;

struct ValueContainer
{
private:
    int value;

public:
    ValueContainer(int value)
    {
        this->value = value;
    }

    int getValue()
    {
        return this->value;
    }
};

struct TrieNode
{
private:
    char letter;
    ValueContainer* value;
    TrieNode* parent;
    TrieNode** children;

    int findChildIndex(char letter)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr && node->letter == letter)
                return i;
        }

        return -1;
    }

public:
    TrieNode(char letter, ValueContainer* value = nullptr)
    {
        this->letter = letter;
        this->value = value;
        this->parent = nullptr;
        this->children = new TrieNode * [ALPHABET_SIZE];

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->children[i] = nullptr;
        }
    }

    TrieNode* findChild(char letter)
    {
        int index = this->findChildIndex(letter);

        return index > -1 ? this->children[index] : nullptr;
    }

    TrieNode* addChild(char letter, ValueContainer* value = nullptr)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (!this->children[i])
            {
                TrieNode* node = new TrieNode(letter, value);
                node->parent = this;

                this->children[i] = node;

                return node;
            }
        }
        return nullptr;
    }

    void deleteChild(char letter)
    {
        int index = this->findChildIndex(letter);
        if (index == -1)
            return;

        delete this->children[index];
        this->children[index] = nullptr;
    }

    void deleteSelf()
    {
        if (this->parent)
            this->parent->deleteChild(this->letter);

        // call destructor
    }

    bool hasChildren()
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr)
                return true;
        }

        return false;
    }

    void setValue(ValueContainer* value)
    {
        this->value = value;
    }

    ValueContainer* getValue()
    {
        return this->value;
    }

    TrieNode* getParent()
    {
        return this->parent;
    }

    char getLetter()
    {
        return this->letter;
    }
    void print(TrieNode* node, string str)
    {
        TrieNode* current;
        current = node;
        if (node != nullptr)
        {
            str = str + current->letter;
            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (current->children[i] != nullptr)
                {
                    current = current->children[i];
                    print(current, str);
                    current = current->parent;
                }
            }
            if (current->getValue() != nullptr)
                cout << "\n" << str;
        }

    }
};

struct TrieTree
{
private:
    TrieNode* root;
    int count = 0;
    TrieNode* getNode(string key)
    {
        TrieNode* node = this->root;

        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            node = node->findChild(letter);
            if (!node)
                return nullptr;
        }

        return node;
    }

public:
    TrieTree()
    {
        this->root = new TrieNode(ROOT_LETTER);
    }

    ValueContainer* get(string key)
    {
        TrieNode* node = this->getNode(key);

        return node ? node->getValue() : nullptr;
    }

    bool set(string key, ValueContainer* value, vector<char> consonants)
    {
        TrieNode* node = this->root;
        for (int l = 0; l < consonants.size(); l++)
        {
            if (key[key.length() - 1] == consonants[l]) count++;
        }

        bool isNewBranch = false;

        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            TrieNode* next = isNewBranch ? node->findChild(letter) : nullptr;
            if (next)
                node = next;
            else
            {
                node = node->addChild(letter);
                isNewBranch = true;
            }
        }

        if (node)
            node->setValue(value);

        return node != nullptr;
    }

    bool remove(string key)
    {
        TrieNode* node = this->getNode(key);
        if (!node)
            return false;

        TrieNode* current = node;

        while (current && current->getLetter() != ROOT_LETTER)
        {
            TrieNode* parent = current->getParent();

            if (current->hasChildren())
            {
                if (current == node)
                    current->setValue(nullptr);
                return true;
            }

            current->deleteSelf();
            current = parent;
        }

        return true;
    }
    void print()
    {
        string str;
        root->print(root, str);
    }
    int getcount()
    {
        return count;
    }

};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector <char> cc{ 'б', 'в', 'г', 'д', 'ж', 'з', 'й', 'к', 'л', 'м', 'н', 'п', 'р', 'с', 'т', 'ф', 'х', 'ц', 'ч', 'ш', 'щ' };
    TrieTree tree;
    int i = 1;
    string word;
    cout << "Введите слова: ";
    cin >> word;
    while (word != "0")
    {
        ValueContainer value(i);
        tree.set(word, &value, cc);
        cin >> word;
        i++;
    }
    tree.print();

    cout << "\nКоличество слов, оканчивающихся на согласную: " << tree.getcount() << endl;
    return 0;

}