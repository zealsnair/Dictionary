#include <bits/stdc++.h>
using namespace std;

#define total_alphabets 26
#define underline_start "\033[4m"
#define underline_stop "\033[0m"

int letter_count(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }

    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }

    return -1;
}

struct temp
{
    char word[20], meaning[50];
};

class trienode
{
public:
    char meaning[50];
    trienode *edge[total_alphabets];
    int end_indicator;

    trienode()
    {
        end_indicator = 0;

        for (int i = 0; i < total_alphabets; i++)
        {
            edge[i] = NULL;
        }
    }

} *root = new trienode();

void read_from_file()
{
    ifstream file("dictionary.dat", ios::binary | ios::in);
    temp obj;

    while (file.read((char *)&obj, sizeof(obj)))
    {

        trienode *current = root;
        char temp[20];
        strcpy(temp, obj.word);
        int index;
        for (int i = 0; i < strlen(temp); i++)
        {
            index = letter_count(temp[i]);

            if (!current->edge[index])
                current->edge[index] = new trienode();

            current = current->edge[index];
        }

        current->end_indicator = 1;
        strcpy(current->meaning, obj.meaning);
    }
    file.close();
}

void insert(char word[], char meaning[])
{
    temp object;
    trienode *current = root;
    int index;

    for (int i = 0; i < strlen(word); i++)
    {
        index = letter_count(word[i]);

        if (!current->edge[index])
            current->edge[index] = new trienode();

        current = current->edge[index];
    }
    current->end_indicator = 1;
    strcpy(current->meaning, meaning);

    strcpy(object.word, word);
    strcpy(object.meaning, meaning);

    ofstream file("dictionary.dat", ios::binary | ios::app);

    file.write((char *)&object, (sizeof(object)));

    file.close();
}

bool search(char word[], int flag)
{
    trienode *current = root;
    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = letter_count(word[i]);
        if (current->edge[index] == NULL)
            return false;

        current = current->edge[index];
    }

    if (current->end_indicator != 1)
        return false;
    if (flag == 1)
    {
        cout << "Meaning of this word is ";
        cout << current->meaning;
    }
    return true;
}

bool delete_word(char word[])
{
    temp object;
    trienode *current = root;

    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = letter_count(word[i]);
        if (current->edge[index] == NULL)
            return false;

        current = current->edge[index];
    }

    if (current->end_indicator == 1)
    {
        current->end_indicator = 0;
    }
    else
    {
        return false;
    }

    ifstream file("dictionary.dat", ios::binary | ios::in);
    ofstream new_file("temporary.dat", ios::out | ios::binary);

    while (file.read((char *)&object, sizeof(object)))
    {
        if (strcmp(object.word, word) == 1)
            new_file.write((char *)&object, sizeof(temp));
    }

    file.close();
    new_file.close();
    remove("dictionary.dat");
    rename("temporary.dat", "dictionary.dat");
    cout << "\nWord is successfully deleted.";
    return true;
}

void print_prefix_search(trienode *node, char allWords[], int index, char prefix[])
{
    trienode *curr = node;

    if (curr != NULL && curr->end_indicator == 1)
    {
        cout << prefix;

        for (int j = 0; j < index; j++)
        {
            cout << allWords[j];
        }

        cout << ": ";
        cout << curr->meaning << "\n";
    }

    for (int i = 0; i < total_alphabets; i++)
    {
        if (curr->edge[i] != NULL)
        {
            allWords[index] = 'a' + i;
            print_prefix_search(curr->edge[i], allWords, index + 1, prefix);
        }
    }
}

void prefix_search(char prefix[])
{
    trienode *curr = root;

    for (int i = 0; i < strlen(prefix); i++)
    {
        int index = letter_count(prefix[i]);
        curr = curr->edge[index];
    }

    char allWords[50];
    print_prefix_search(curr, allWords, 0, prefix);
}

void spell_checker(char phrase[])
{

    int i = 0, j = 0;
    while (phrase[i] != '\0')
    {
        char temp[20] = "";
        j = 0;
        while (phrase[i] != ' ' && phrase[i] != '\0')
        {
            temp[j] = phrase[i];
            i++;
            j++;
        }

        if (!search(temp, 0))
        {
            cout << underline_start << temp << underline_stop << " ";
        }

        else
        {
            cout << temp << " ";
        }
        if (phrase[i] == '\0')
            break;
        i++;
    }
}

bool compound(char word[], int index, int sections)
{
    if (index == strlen(word))
        return sections >= 2;

    trienode *cur = root;

    for (int i = index; i < strlen(word); i++)
    {
        int index = letter_count(word[i]);

        if (cur->edge[index])
        {
            cur = cur->edge[index];
            if (cur->end_indicator && compound(word, i + 1, sections + 1))
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}

int main()
{
    read_from_file();
    char word[50], meaning[50];
    int choice;
    char ans;
    do
    {
        cout << "\n\tDictionary\n\n\n"
             << "\t1. Search a word\n"
             << "\t2. Add a word\n"
             << "\t3. Delete a word\n"
             << "\t4. Prefix search or autocomplete\n"
             << "\t5. Spell Checker\n"
             << "\t6. Compound Checker\n"
             << "\t7. EXIT\n";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\nEnter the word to be searched : ";
            fflush(stdin);
            cin.get(word, 20);
            if (!search(word, 1))
            {
                cout << "\nWord is not found. \nTry again";
            }
            break;

        case 2:
            cout << "Enter the word to be added: ";
            fflush(stdin);
            cin.get(word, 20);

            cout << "Enter its meaning: ";
            fflush(stdin);
            cin.get(meaning, 50);

            insert(word, meaning);
            break;

        case 3:
            cout << "Enter the word to be deleted: ";
            fflush(stdin);
            cin.get(word, 20);
            if (!delete_word(word))
            {
                cout << "\nWord to be deleted is not found.";
            }
            break;

        case 4:
            cout << "Enter the prefix: ";
            fflush(stdin);
            cin.get(word, 20);
            prefix_search(word);
            break;

        case 5:
            char phrase[100];
            cout << "Enter any word or phrase: ";
            fflush(stdin);
            cin.get(phrase, 100);
            spell_checker(phrase);
            break;

        case 6:
            cout << "Enter the word to be checked: ";
            fflush(stdin);
            cin.get(word, 20);
            if (compound(word, 0, 0))
                cout << "true";

            else
                cout << "false";
            break;

        case 7:
            exit(0);
            break;
        }

        cout << "\nDo you want to continue : ";
        cin >> ans;
    } while (ans != 'n' && ans != 'N');

    return 0;
}
