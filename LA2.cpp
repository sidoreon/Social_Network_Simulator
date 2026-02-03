#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>
#include <cctype>
#include <limits>

using namespace std;

struct Post{
    string content;
    int time;
    Post(string c, int t) : content(c), time(t){}
};

struct Node{
    Post* post;
    int height;
    Node* left;
    Node* right;
    Node(Post* p) : post(p), height(1), left(nullptr), right(nullptr){}
};

class Tree{
private:
    Node* root;
    
    int getheight(Node* node){
        if (node) return node->height;
        else return 0;
    }
    
    int getbalance(Node* node){
        if (node) return getheight(node->left) - getheight(node->right);
        else return 0;
    }
    
    void updateheight(Node* node){
        if (node) node->height = 1 + max(getheight(node->left), getheight(node->right));
        else return;
    }
    
    Node* rightrotate(Node* y){
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateheight(y);
        updateheight(x);
        return x;
    }
    
    Node* leftrotate(Node* x){
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateheight(x);
        updateheight(y);
        return y;
    }
    
    Node* insert(Node* node, Post* post){
        if (!node) return new Node(post);
        if (post->time < node->post->time){
            node->left = insert(node->left, post);
        } else {
            node->right = insert(node->right, post);
        }
        updateheight(node);
        int balance = getbalance(node);
        if (balance > 1 && post->time < node->left->post->time){
            return rightrotate(node);
        }
        if (balance < -1 && post->time >= node->right->post->time){
            return leftrotate(node);
        }
        if (balance > 1 && post->time >= node->left->post->time){
            node->left = leftrotate(node->left);
            return rightrotate(node);
        }
        if (balance < -1 && post->time < node->right->post->time){
            node->right = rightrotate(node->right);
            return leftrotate(node);
        }
        return node;
    }
    
    void revinorder(Node* node, vector<Post*>& posts){
        if (!node) return;
        revinorder(node->right, posts);
        posts.push_back(node->post);
        revinorder(node->left, posts);
    }
    
public:
    Tree() : root(nullptr){}
    
    void insert(Post* post){
        root = insert(root, post);
    }
    
    vector<Post*> revposts(){
        vector<Post*> posts;
        revinorder(root, posts);
        return posts;
    }
};

struct User{
    string username;
    string casename;
    vector<User*> friends;
    Tree* posts;
    User(string name, string casen) : username(name), casename(casen), posts(new Tree()){}
};

class socialnet{
private:
    unordered_map<string, User*> users;
    int postctr;
    
    string tolowerstr(string str){
        string result;
        for (int i = 0; i < (int)str.size(); i++){
            result += tolower(str[i]);
        }
        return result;
    }
    
public:
    socialnet() : postctr(0){}
    
    void adduser(string username){
        string actun = username;
        username = tolowerstr(username);
        if (users.find(username) == users.end()){
            users[username] = new User(username, actun);
            cout << "User " << actun << " added." << endl;
        } else {
            cout << "ERROR: User " << actun << " already exists." << endl;
        }
    }
    
    void addfriend(string username1, string username2){
        string actun1 = username1;
        string actun2 = username2;
        username1 = tolowerstr(username1);
        username2 = tolowerstr(username2);
        if (username1 == username2){
            cout << "ERROR: Cannot friend yourself." << endl;
            return;
        }
        if (users.find(username1) == users.end() && users.find(username2) == users.end()){
            cout << "ERROR: Users " << actun1 << " and " << actun2 << " do not exist." << endl;
            return;
        }
        if (users.find(username1) == users.end()){
            cout << "ERROR: User " << actun1 << " does not exist." << endl;
            return;
        }
        if (users.find(username2) == users.end()){
            cout << "ERROR: User " << actun2 << " does not exist." << endl;
            return;
        }
        User* user1 = users[username1];
        User* user2 = users[username2];
        int flag = 0;
        for (User* f : user1->friends){
            if (f->username == username2){
                flag = 1;
                break;
            }
        }
        if (!flag){
            user1->friends.push_back(user2);
            user2->friends.push_back(user1);
            cout << "Friend added." << endl;
        } else {
            cout << "ERROR: Friend already exists." << endl;
        }
    }

    void listfriends(string username){
        string actun = username;
        username = tolowerstr(username);
        if (users.find(username) == users.end()){
            cout << "ERROR: User " << actun << " does not exist." << endl;
            return;
        }
        User* user = users[username];
        vector<string> friendnames; 
        for (User* f : user->friends){
            friendnames.push_back(f->casename);
        }
        sort(friendnames.begin(), friendnames.end());
        for (string name : friendnames){
            cout << name << endl;
        }
    }
    
    void suggestfriends(string username, int N){
        string actun = username;
        username = tolowerstr(username);
        if (N == 0) return;
        if (users.find(username) == users.end()){
            cout << "ERROR: User " << actun << " does not exist." << endl;
            return;
        }
        User* user = users[username];
        unordered_map<string, int> mutualcnt;
        unordered_map<string, bool> isfriend;
        isfriend[username] = true;
        for (User* f : user->friends){
            isfriend[f->username] = true;
        }
        for (User* frienduser : user->friends){
            for (User* fof : frienduser->friends){
                if (!isfriend[fof->username]){
                    mutualcnt[fof->username]++;
                }
            }
        }
        vector<pair<int, string>> suggests;
        for (auto p : mutualcnt){
            suggests.push_back({-p.second, users[p.first]->casename}); 
        }
        if (suggests.size() == 0){
            cout << "ERROR: No mutual friends" << endl;
            return;
        }
        sort(suggests.begin(), suggests.end());
        int count = 0;
        for (auto p : suggests){
            if (count >= N) break;
            cout << p.second << endl;
            count++;
        }
    }
    
    void dos(string username1, string username2){
        string actun1 = username1;
        string actun2 = username2;
        username1 = tolowerstr(username1);
        username2 = tolowerstr(username2);
        if (users.find(username1) == users.end() && users.find(username2) == users.end()){
            cout << "ERROR: Users " << actun1 << " and " << actun2 << " do not exist." << endl;
            return;
        }
        if (users.find(username1) == users.end()){
            cout << "ERROR: User " << actun1 << " does not exist." << endl;
            return;
        }
        if (users.find(username2) == users.end()){
            cout << "ERROR: User " << actun2 << " does not exist." << endl;
            return;
        }
        if (username1 == username2){
            cout << 0 << endl;
            return;
        }
        User* start = users[username1];
        User* end = users[username2];
        unordered_map<string, bool> visited;
        queue<pair<User*, int>> q;
        q.push({start, 0});
        visited[start->username] = true;
        while (!q.empty()){
            User* current = q.front().first;
            int dist = q.front().second;
            q.pop();
            if (current->username == end->username){
                cout << dist << endl;
                return;
            }
            for (User* frienduser : current->friends){
                if (!visited[frienduser->username]){
                    visited[frienduser->username] = true;
                    q.push({frienduser, dist + 1});
                }
            }
        }
        cout << -1 << endl;
    }
    
    void addpost(string username, string content){
        string actun = username;
        username = tolowerstr(username);
        if (users.find(username) == users.end()){
            cout << "ERROR: User " << actun << " does not exist." << endl;
            return;
        }
        if (content.empty()){
            cout << "ERROR: Post content cannot be empty." << endl;
            return;
        }
        Post* post = new Post(content, postctr++);
        users[username]->posts->insert(post);
        cout << "Post " << content << " added for " << actun << "." << endl;
    }

    void outputposts(string username, int N){
        string actun = username;
        username = tolowerstr(username);
        if (users.find(username) == users.end()){
            cout << "ERROR: User " << actun << " does not exist." << endl;
            return;
        }
        vector<Post*> posts = users[username]->posts->revposts();
        int count;
        if (N == -1){
            count = posts.size();
        } else {
            count = min(N, (int)posts.size());
        }
        for (int i = 0; i < count; i++){
            cout << posts[i]->content << endl;
        }
    }
};

int main(){
    socialnet net;
    string line;
    cout << "SocialNet active." << endl << "Input EXIT to quit" << endl;
    while (getline(cin, line)){
        if (line.empty()) continue;
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if (cmd == "ADD_USER"){
            string username;
            if (!(ss >> username)){
                cout << "ERROR: Missing username." << endl;
            } else {
                net.adduser(username);
            }
        }
        else if (cmd == "ADD_FRIEND"){
            string user1, user2;
            if (!(ss >> user1 >> user2)){
                cout << "ERROR: Missing username(s)." << endl;
            } else {
                net.addfriend(user1, user2);
            }
        }
        else if (cmd == "LIST_FRIENDS"){
            string username;
            if (!(ss >> username)){
                cout << "ERROR: Missing username." << endl;
            } else {
                net.listfriends(username);
            }
        }
        else if (cmd == "SUGGEST_FRIENDS"){
            string username;
            int n;
            if (!(ss >> username >> n)){
                cout << "ERROR: Missing username or number." << endl;
            } else if (n < -1){
                cout << "ERROR: Invalid number." << endl;
            } else {
                net.suggestfriends(username, n);
            }
        }
        else if (cmd == "DEGREES_OF_SEPARATION"){
            string user1, user2;
            if (!(ss >> user1 >> user2)){
                cout << "ERROR: Missing username(s)." << endl;
            } else {
                net.dos(user1, user2);
            }
        }
        else if (cmd == "ADD_POST"){
            string username;
            if (!(ss >> username)){
                cout << "ERROR: Missing username." << endl;
                continue;
            }
            string content;
            getline(ss, content);
            if (!content.empty() && content[0] == ' '){
                content = content.substr(1);
            }
            if (content.empty()){
                cout << "ERROR: Missing post content." << endl;
            } else {
                net.addpost(username, content);
            }
        }
            
        else if (cmd == "OUTPUT_POSTS"){
            string username;
            int n;
            if (!(ss >> username >> n)){
                cout << "ERROR: Missing username or number." << endl;
            } else if (n < -1){
                cout << "ERROR: Invalid number." << endl;
            } else {
                net.outputposts(username, n);
            }
        }
        else if (cmd == "EXIT"){
            cout << "Exiting" << endl;
            break;
        }
        else {
            cout << "ERROR: Invalid command." << endl;
        }
    }
    return 0;
}