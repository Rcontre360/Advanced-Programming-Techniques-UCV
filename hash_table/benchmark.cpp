#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

//initialized here to avoid wasting time
//because this is used during the benchmark
random_device rd;
mt19937 gen(rd());
int rand(int start,int end){
    uniform_int_distribution<> dist(start,end); 
    return dist(gen);
}

string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
string randString(size_t length) {
    string res;
    for (size_t i = 0; i < length; ++i) {
        res += chars[rand(0,chars.size()-1)];
    }

    return res;
}

vector<pair<string,int>> randPairs(int amount){
    vector<pair<string,int>> res;

    for (int i=0; i<amount; i++)
        res.push_back(make_pair(randString(15),rand(-1000000,1000000)));

    return res;
}

struct UnorderedSetup{
    unordered_map<string,int> mp; //map structure filled
    vector<pair<string,int>> inserted; //all elements inserted
    int size; //the N of the test

    UnorderedSetup(
        unordered_map<string,int> mp, 
        int size, 
        vector<pair<string,int>> pairs
    ) : mp(mp),inserted(pairs),size(size) {}
};

UnorderedSetup setupUnorderedMap(int size){
    unordered_map<string,int> mp;
    auto pairs = randPairs(size);

    for (auto x:pairs)
        mp[x.first] = x.second;

    return UnorderedSetup(mp,size,pairs);
}

struct MapSetup{
    map<string,int> mp; //map structure filled
    vector<pair<string,int>> inserted; // all elements inserted
    int size; //the N of the test

    MapSetup(
        map<string,int> mp, 
        int size, 
        vector<pair<string,int>> pairs
    ) : mp(mp),inserted(pairs),size(size){}
};

MapSetup setupMap(int size){
    map<string,int> mp;
    auto pairs = randPairs(size);

    for (auto x:pairs)
        mp[x.first] = x.second;

    return MapSetup(mp,size, pairs);
}

int unorderedMapRemove(UnorderedSetup setup){
    return 0;
}

int mapRemove(MapSetup setup){
    return 0;
}

int unorderedMapSearch(UnorderedSetup setup){
    return 0;
}

int mapSearch(MapSetup setup){
    return 0;
}

int unorderedMapInsert(UnorderedSetup setup){
    return 0;
}

int mapInsert(MapSetup setup){
    return 0;
}

double unorderedMapRandomOp(UnorderedSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        //operation to perform
        int op = rand(1,3);
        //random index of all inserted elements
        int indx = rand(0,setup.inserted.size());
        auto inserted = setup.inserted[indx];
        //random to insert or search for
        auto randKeyVal = randPairs(1)[0];

        auto start = high_resolution_clock::now(); // Placeholder, will be reset in each case
        //we only benchmark the switch
        switch (op){
            case 1: //insert
                setup.mp[randKeyVal.first] = randKeyVal.second;
                break;
            case 2: //search
                setup.mp[inserted.first];
                break;
            case 3: //remove
                setup.mp.erase(inserted.first);
                break;
        }
        auto end = high_resolution_clock::now();   // Placeholder, will be reset in each case

        //we need to remove or add the element from the list. To avoid errors on the tests cases
        if (op == 3){
            setup.inserted.erase(setup.inserted.begin() + indx);
        }
        if (op == 1){
            setup.inserted.push_back(randKeyVal);
        }

        totalTime += (end-start).count();
    }

    //return average time
    return totalTime / times;
}

double mapRandomOp(MapSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        //operation to perform
        int op = rand(1,3);
        //random index of all inserted elements
        int indx = rand(0,setup.inserted.size());
        auto inserted = setup.inserted[indx];
        //random to insert or search for
        auto randKeyVal = randPairs(1)[0];

        auto start = high_resolution_clock::now(); // Placeholder, will be reset in each case
        //we only benchmark the switch
        switch (op){
            case 1: //insert
                setup.mp[randKeyVal.first] = randKeyVal.second;
                break;
            case 2: //search
                setup.mp[inserted.first];
                break;
            case 3: //remove
                setup.mp.erase(inserted.first);
                break;
        }
        auto end = high_resolution_clock::now();   // Placeholder, will be reset in each case

        //we need to remove or add the element from the list. To avoid errors on the tests cases
        if (op == 3){
            setup.inserted.erase(setup.inserted.begin() + indx);
        }
        if (op == 1){
            setup.inserted.push_back(randKeyVal);
        }

        totalTime += (end-start).count();
    }

    //return average time
    return totalTime / times;
}

template <typename Func,typename Setup>
void benchmarkAndLog(
    const string& hash_map,
    const string& measure,
    int size,
    ofstream& file,
    Func operation,
    Setup setup
){
    double time = operation(setup(size));

    cout << "done: " << hash_map << "|" << size << endl;
    file << hash_map << "," << measure << "," << size << "," << time << "\n";
}


int main() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream mapFile("./benchmarks/map.csv");
    ofstream unorderedFile("./benchmarks/unordered_map.csv");

    mapFile << "Type,Measure,Size,Time\n";
    unorderedFile << "Type,Measure,Size,Time\n";

    for (int i=10000; i<100000; i+=10000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[8];

        threads[0] =  thread([=, &mapFile]() {
            benchmarkAndLog("Map", "AllRandom", size, ref(mapFile), mapRandomOp, setupMap);
        });

        threads[1] =  thread([=, &unorderedFile]() {
            benchmarkAndLog("UnorderedMap","AllRandom", size, ref(unorderedFile), unorderedMapRandomOp, setupUnorderedMap);
        });

        for (auto& th : threads) {
            th.join();
        }
    }

    mapFile.close();
    unorderedFile.close();

    return 0;
}

