#include <iostream>
#include <limits>
using namespace std;


bool isValidInt(const string &str){
    try {
        stoi(str);
        return true;
    }
    catch (const invalid_argument&) {
        cout << "\nInvalid input: '" << str << "' is not a number.\n";
    }
    catch (const out_of_range&) {
        cout << "\nInput out of range: '" << str << "' is too large or too small.\n";
    }
    return false;
}


string toLowerCase(string &str) {
    for (int i = 0; i < str.length(); i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + ('a' - 'A');
    return str;
}


class drugs {
public:

    string ID , name , price , inventory , category;

    drugs* left;
    drugs* right;

    drugs() {
        left = right = nullptr;
    }
    ~drugs() = default;
};

drugs* cheapest = nullptr , * mostExpensive = nullptr;
int totalInventory = 0 , drugCount = 0 , categoryCount = 0;

class category {

public:
    drugs* node;
    string categoryName;


    category* left;
    category* right;

    category(string ctgName) {
        categoryName = ctgName;
        node = nullptr;
        left = right = nullptr;
    }
    ~category() = default;
};

class trie {
    public:
        trie * child[27];
        bool isEndOfWord;
        trie() {
            isEndOfWord = false;
            for (int i = 0; i < 27; i++)
                child[i] = nullptr;
        }
        ~trie() {
            for (int i = 0; i < 27; i++)
                if (child[i] != nullptr)
                    delete child[i];
        }
};

drugs* searchName(drugs* root, string drugName);


void displayNode(const drugs* root) {
    cout<<"Name: "<<root->name
        <<" - ID: "<<root->ID
        <<" - Price: "<<root->price
        <<" - Inventory: "<<root->inventory<<endl;
}


drugs* createNode(drugs* root) {

    auto* newNode = new drugs;
    newNode->left = newNode->right = nullptr;

    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    cout<<"ID: ";
    getline(cin,newNode->ID);
    if (!isValidInt(newNode->ID)) {
        delete newNode;
        return nullptr;
    }

    cout<<"Name: ";
    getline(cin,newNode->name);
    newNode->name = toLowerCase(newNode->name);

    drugs* existingDrug = searchName(root , newNode->name);

    if ( existingDrug != nullptr) {
        cout<<"\nThis drug Is already Added !!!\n";
        displayNode(existingDrug);
        delete newNode;
        return nullptr;
    }

    cout<<"Price: ";
    getline(cin,newNode->price);
    if (!isValidInt(newNode->price)) {
        delete newNode;
        return nullptr;
    }


    cout<<"Inventory: ";
    getline(cin,newNode->inventory);
    if (!isValidInt(newNode->inventory)){
        delete newNode;
        return nullptr;
    }

    cout<<"Category: ";
    getline(cin,newNode->category);
    newNode->category = toLowerCase(newNode->category);

    return newNode;
}


drugs* maximumID(drugs* root) {
    if (root == nullptr) {
        cout<<"\nEmpty !!!\n";
        return nullptr;
    }

    if (root->right == nullptr) {
        return root;
    }
    return maximumID(root->right);
}


drugs* minimumID(drugs* root) {

    if (root == nullptr) {
        cout<<"\nEmpty !!!\n";
        return nullptr;
    }

    if (root->left == nullptr) {
        return root;
    }

    return minimumID(root->left);
}


void maximumPrice(drugs* root) {
    if (root == nullptr) {
        return;
    }

    if (root->price > mostExpensive->price)
        mostExpensive = root;

    maximumPrice(root->left);
    maximumPrice(root->right);
}


void minimumPrice(drugs* root) {
    if (root == nullptr) {
        return;
    }

    if (root->price < cheapest->price)
        cheapest = root;

    minimumPrice(root->left);
    minimumPrice(root->right);
}


category* minimumCategory(category* root) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->left == nullptr) {
        return root;
    }

    return minimumCategory(root->left);
}


void insertDrug (drugs* &subtree ,drugs* newNode, bool updateMinMax) {

    if (subtree == nullptr) {
        if (updateMinMax){
            totalInventory += stoi(newNode->inventory);
            drugCount++;

            if (cheapest == nullptr || mostExpensive == nullptr) {
                cheapest = newNode;
                mostExpensive = newNode;
            }
            else {
                if (stoi(mostExpensive->price) < stoi(newNode->price))
                    mostExpensive = newNode;
                if (stoi(cheapest->price) > stoi(newNode->price))
                    cheapest = newNode;
            }
        }
        subtree = newNode;
        return;
    }

    if (stoi(subtree->ID) > stoi(newNode->ID))
        insertDrug(subtree->left, newNode, updateMinMax);
    else
        insertDrug(subtree->right, newNode, updateMinMax);

}


category* findCategory(category* root , string ctgName) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->categoryName == ctgName) {
        return root;
    }

    if (ctgName < root->categoryName) {
        return findCategory(root->left, ctgName);
    }

    return findCategory(root->right, ctgName);
}


void insertCategory(category* &root,category* newNode) {
    if (root == nullptr) {
        root = newNode;
        return;
    }

    if (root->categoryName > newNode->categoryName)
        insertCategory(root->left,newNode);
    else if (root->categoryName < newNode->categoryName)
        insertCategory(root->right,newNode);
}



drugs* searchID(drugs* root,string ID) {
    if (root == nullptr)
        return nullptr;

    if (root->ID == ID )
        return root;

    if (stoi(root->ID) > stoi(ID) )
        return searchID(root->left,ID);

    return searchID(root->right,ID);
}

drugs* searchName(drugs* root, string drugName) {
    if (root == nullptr)
        return nullptr;

    if (root->name == drugName)
        return root;

    drugs* result = searchName(root->left, drugName);
    if (result != nullptr)
        return result;

    return searchName(root->right, drugName);
}

void deleteNode(drugs* &root, drugs* target, bool shouldUpdate) {
    if (root == nullptr) return;

    bool isMaxPrice = false, isMinPrice = false;

    if (shouldUpdate) {
        isMaxPrice = (target == mostExpensive);
        isMinPrice = (target == cheapest);
        totalInventory -= stoi(target->inventory);
        drugCount--;
    }

    drugs* parent = nullptr;
    drugs* current = root;

    while (current != nullptr && current->ID != target->ID) {
        parent = current;
        if (stoi(target->ID) < stoi(current->ID))
            current = current->left;
        else
            current = current->right;
    }

    // Leaf
    if (current->left == nullptr && current->right == nullptr) {
        if (current == root)
            root = nullptr;
        else {
            if (parent->left == current)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
    }

        // have right child
    else if (current->left == nullptr) {
        if (current == root)
            root = current->right;
        else {
            if (parent->left == current)
                parent->left = current->right;
            else
                parent->right = current->right;
        }
    }

        // have left child
    else if (current->right == nullptr) {
        if (current == root)
            root = current->left;
        else {
            if (parent->left == current)
                parent->left = current->left;
            else
                parent->right = current->left;
        }
    }
        // have both
    else {
        drugs* successor = minimumID(current->right);
        drugs tempSuccessor = *successor;

        deleteNode(root, successor, false);

        current->ID = tempSuccessor.ID;
        current->name = tempSuccessor.name;
        current->price = tempSuccessor.price;
        current->inventory = tempSuccessor.inventory;
        current->category = tempSuccessor.category;
    }

    if (shouldUpdate && (isMaxPrice || isMinPrice)) {
        if (root != nullptr) {
            if (isMaxPrice) {
                mostExpensive = root;
                maximumPrice(root);
            }
            if (isMinPrice) {
                cheapest = root;
                minimumPrice(root);
            }
        } else {
            mostExpensive = nullptr;
            cheapest = nullptr;
        }
    }
}

void deleteNodeCtg(category* &root, category* target) {
    if (root == nullptr) return;

    category* parent = nullptr;
    category* current = root;

    while (current != nullptr && current->categoryName != target->categoryName) {
        parent = current;
        if (target->categoryName < current->categoryName)
            current = current->left;
        else
            current = current->right;
    }

    // Leaf
    if (current->left == nullptr && current->right == nullptr) {
        if (current == root) {
            delete root;
            root = nullptr;
        } else {
            if (parent->left == current)
                parent->left = nullptr;
            else
                parent->right = nullptr;
            delete current;
        }
    }

        // have right child
    else if (current->left == nullptr) {
        if (current == root) {
            root = current->right;
        } else {
            if (parent->left == current)
                parent->left = current->right;
            else
                parent->right = current->right;
        }
        delete current;
    }

        // have left child
    else if (current->right == nullptr) {
        if (current == root) {
            root = current->left;
        } else {
            if (parent->left == current)
                parent->left = current->left;
            else
                parent->right = current->left;
        }
        delete current;
    }

        // both
    else {
        category* successor = minimumCategory(current->right);
        current->categoryName = successor->categoryName;
        current->node = successor->node;

        if (successor == current->right)
            current->right = successor->right;
        else {
            category* successorParent = current->right;
            while (successorParent->left != successor)
                successorParent = successorParent->left;
            successorParent->left = successor->right;
        }
        delete successor;
    }

    categoryCount--;
}


void deleteAll(drugs* &drugRoot, category* &ctgRoot, string ID) {
    // 1. Find target drug
    drugs* target = searchID(drugRoot, ID);

    if (target == nullptr) {
        cout << "\nTarget Not Found !!!\n";
        return;
    }

    // 2. Store category name before deletion
    string targetCategory = target->category;

    // 3. Delete from main drug tree
    deleteNode(drugRoot, target, true);

    // 4. Find category node
    category* categoryNode = findCategory(ctgRoot, targetCategory);

    if (categoryNode == nullptr)
        return;

    // 5. Delete drug from category's internal tree
    drugs* categoryDrug = searchID(categoryNode->node, ID);

    if (categoryDrug != nullptr) {

        deleteNode(categoryNode->node, categoryDrug, false);
        // 6. If category's drug tree becomes empty, delete category node
        if (categoryNode->node == nullptr)
            deleteNodeCtg(ctgRoot, categoryNode);

    }
}

int nodeCount(drugs* root) {
    if (root == nullptr)
        return 0;
    return 1 + nodeCount(root->left) + nodeCount(root->right);
}

void inOrder(const drugs* root) {
    if (root == nullptr)
        return;

    inOrder(root->left);
    displayNode(root);
    inOrder(root->right);
}

void inOrderCtg(const category* root) {
    if (root == nullptr)
        return;

    inOrderCtg(root->left);

    cout << "\n<< Category: " << root->categoryName<<" - Number Of Drugs: "<<nodeCount(root->node)<<" >>" << endl;
    inOrder(root->node);

    inOrderCtg(root->right);
}

int heightDrugs(drugs* root) {
    if (root == nullptr)
        return 0;
    return 1 + max( heightDrugs(root->left), heightDrugs(root->right) );
}

int heightCtg(category* root) {
    if (root == nullptr)
        return 0;
    return 1 + max(heightCtg(root->left) , heightCtg(root->right));
}

bool isBalancedDrugs(drugs* root) {
    if (root == nullptr)
        return true;

    int leftHeight  = heightDrugs(root->left);
    int rightHeight  = heightDrugs(root->right);

    if (abs(leftHeight - rightHeight) <= 1 && isBalancedDrugs(root->left) && isBalancedDrugs(root->right))
        return true;
    return false;
}

bool isBalancedCtg(category* root) {
    if (root == nullptr)
        return true;

    int leftHeight  = heightCtg(root->left);
    int rightHeight  = heightCtg(root->right);

    if (abs(leftHeight - rightHeight) <= 1 && isBalancedCtg(root->left) && isBalancedCtg(root->right))
        return true;
    return false;
}

void checkInventory(const drugs* root,int &counter) {
    if (root == nullptr)
        return;

    checkInventory(root->left,counter);
    counter += stoi(root->inventory);
    checkInventory(root->right,counter);
}


bool existingInRange = false;
void displayWithPrice(const drugs* root,string minRange,string maxRange) {
    if (root == nullptr)
        return;

    displayWithPrice(root->left,minRange,maxRange);

    if (stoi(root->price) >= stoi(minRange) && stoi(root->price) <= stoi(maxRange)) {
        existingInRange = true;
        displayNode(root);
    }

    displayWithPrice(root->right,minRange,maxRange);
}


void deleteDrugTree(drugs* root) {
    if (root == nullptr) return;
    deleteDrugTree(root->left);
    deleteDrugTree(root->right);
    delete root;
}

void deleteCategoryTree(category* root) {
    if (root == nullptr) return;
    deleteCategoryTree(root->left);
    deleteCategoryTree(root->right);
    deleteDrugTree(root->node);  // delete drugs in this category
    delete root;
}




/*  TRIE FUNCTIONS   */

int getCharIndex(char c) {
    return c == ' ' ? 26 : c - 'a';
}

char getCharFromIndex(int index) {
    return index == 26 ? ' ' : 'a' + index;
}

void insertWord(trie* root, string key) {
    if (root == nullptr || key.empty()) {
        return;
    }

    trie* curr = root;

    for (char c : key) {
        int index = getCharIndex(c);
        if (curr->child[index] == nullptr) {
            curr->child[index] = new trie();
        }
        curr = curr->child[index];
    }
    curr->isEndOfWord = true;
}

bool isLastNode(trie* root) {
    if (root == nullptr)
        return false;

    for (int i = 0; i < 27; i++)
        if (root->child[i] != nullptr)
            return true;
    return false;

}

void suggestions(trie* root , string str) {
    if (root == nullptr)
        return;

    if (root->isEndOfWord)
        cout<< str << endl;

    for (int i = 0; i < 27; i++)
        if (root->child[i]) {
            char c = getCharFromIndex(i);
            suggestions(root->child[i] , str + c);
        }
}

int autoComplete(trie* root , string str) {

    if (root == nullptr || str.empty()) {
        return 0;
    }

    trie* curr = root;

    for (char c : str) {

        int index = getCharIndex(c);

        if (curr->child[index] == nullptr)
            return 0;

        curr = curr->child[index];
    }

    if (!isLastNode(curr)) {
        cout<< str << endl;
        return -1;
    }
    suggestions(curr,str);
    return 1;
}






void pharmacyMenu(drugs* &drug, category* &ctg, trie* &t) {
    while (true) {
        char command = '\0';
        cout<<"\n[0]: Exit\n"
            <<"[1]: Insert\n"
            <<"[2]: Delete\n"
            <<"[3]: Search\n"
            <<"[4]: Display\n"
            <<"[5]: Check Inventory\n"
            <<"[6]: Display the Cheapest And Most Expensive\n"
            <<"[7]: Height\n"
            <<"[8]: Auto Complete\n"
            <<"[9]: Display Category\n"
            <<"--> " << flush;
        cin>>command;
        if (command == '0') {
            deleteDrugTree(drug);
            deleteCategoryTree(ctg);
            delete t;
            cout<<"done";
            break;

        }

        if (command == '1') {

            drugs* newNode = createNode(drug);

            if (newNode == nullptr)
                continue;

            insertDrug(drug, newNode, true);

            insertWord(t,newNode->name);

            category* check = findCategory(ctg, newNode->category);

            if (check == nullptr) {

                auto* newCategory = new category(newNode->category);

                auto* drugCopy = new drugs();
                *drugCopy = *newNode;
                drugCopy->left = drugCopy->right = nullptr;

                newCategory->node = drugCopy;
                insertCategory(ctg, newCategory);
                categoryCount++;
            }

            if (check != nullptr) {

                auto* drugCopy = new drugs();
                *drugCopy = *newNode;
                drugCopy->left = drugCopy->right = nullptr;

                insertDrug(check->node, drugCopy, false);
            }
        }


        else if (command == '2') {

            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }
            string ID;

            cout<<"\nEnter ID to Delete:";
            getline(cin>>ws,ID);

            deleteAll(drug,ctg,ID);
        }


        else if (command == '3') {

            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }

            while (true){
                char cmd = '\0';
                cout<<"[0]: Return\n"
                    <<"[1]: Search By ID\n"
                    <<"[2]: Search By Name\n"
                    <<"[3]: Search By Price Range\n";
                cin>>cmd;

                if (cmd == '0')
                    break;

                if (cmd == '1') {
                    string ID;

                    cout<<"\nEnter ID To Search:";
                    getline(cin >> ws,ID);

                    drugs* result = searchID(drug,ID);

                    if (result != nullptr)
                        displayNode(result);
                    else
                        cout<<"\nNOT FOUND!!!\n";

                    break;
                }

                if (cmd == '2') {
                    string name;

                    cout<<"\nEnter Name To Search:";
                    getline(cin >> ws,name);

                    drugs* result = searchName(drug,name);

                    if (result != nullptr)
                        displayNode(result);
                    else
                        cout<<"\nNOT FOUND!!!\n";

                    break;
                }

                if (cmd == '3') {
                    existingInRange = false;
                    string min , max;

                    cout<<"\nminimum Price:";
                    getline(cin>>ws,min);

                    cout<<"\nmaximum Price:";
                    getline(cin>>ws,max);

                    displayWithPrice(drug,min,max);

                    if (!existingInRange)
                        cout<<"\nNo drugs found in the price range "<< min <<" to " << max<<endl;

                    break;
                }

                else {
                    cout<<"\nInvalid !\n";
                    continue;
                }
            }
        }

        else if (command == '4') {
            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }
            inOrder(drug);
        }

        else if (command == '5') {
            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }

            cout<<"\nNumber Of Drugs: "<<drugCount
                <<"\nTotal Number Of Drugs: "<<totalInventory
                <<"\nNumber Of Categories: "<<categoryCount<<endl;
        }

        else if (command == '6') {
            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }

            if (cheapest == mostExpensive){
                cout<<"\nThere is only One Drug:\n";
                displayNode(cheapest);
                continue;
            }

            cout<<"\nThe Cheapest Drug:\n";
            displayNode(cheapest);

            cout<<"\nThe Most Expensive Drug:\n";
            displayNode(mostExpensive);
        }

        else if (command == '7') {
            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }
            cout<<"\nHeight of The Drugs Tree: "<<heightDrugs(drug);
            if (isBalancedDrugs(drug))
                cout<<"\nBalance Tree\n";
            else
                cout<<"\nNot Balance\n";

            cout<<"\nHeight of The Category Tree: "<<heightCtg(ctg);
            if (isBalancedCtg(ctg))
                cout<<"\nBalance Tree\n";
            else
                cout<<"\nNot Balance\n";
        }

        else if (command == '8') {
            if (drug == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }
            string str;
            cout<<"\nEnter To search:\n";
            getline(cin>>ws,str);

            autoComplete(t,str);
        }

        else if (command == '9') {
            if (drug == nullptr && ctg == nullptr) {
                cout<<"\nNo Drug Available !!!\n";
                continue;
            }
            inOrderCtg(ctg);
        }
        else {
            cout<<"\nChoose Available Option\n";
        }
    }
}


int main(){

    drugs* drug = nullptr;
    category* ctg = nullptr;
    trie* t = new trie();

    pharmacyMenu(drug, ctg, t);


    return 0;
}
