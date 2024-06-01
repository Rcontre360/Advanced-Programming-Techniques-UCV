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

double unorderedMapRemove(UnorderedSetup setup){
    double totalTime = 0;

    for (auto remove:setup.inserted){
        auto start = high_resolution_clock::now(); 
        setup.mp.erase(remove.first);
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    return totalTime;
}

int mapRemove(MapSetup setup){
    double totalTime = 0;

    for (auto remove:setup.inserted){
        auto start = high_resolution_clock::now(); 
        setup.mp.erase(remove.first);
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    return totalTime;
}

int unorderedMapSearch(UnorderedSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        int indx = rand(0,setup.inserted.size()-1);
        auto inserted = setup.inserted[indx];

        auto start = high_resolution_clock::now(); 
        setup.mp[inserted.first];
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    return totalTime;

}

int mapSearch(MapSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        int indx = rand(0,setup.inserted.size()-1);
        auto inserted = setup.inserted[indx];

        auto start = high_resolution_clock::now(); 
        setup.mp[inserted.first];
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    return totalTime;
}

int unorderedMapInsert(UnorderedSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        auto randKeyVal = randPairs(1)[0];

        auto start = high_resolution_clock::now(); 
        setup.mp[randKeyVal.first] = randKeyVal.second;
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    return totalTime;
}

int mapInsert(MapSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times; i++){
        auto randKeyVal = randPairs(1)[0];

        //measure
        auto start = high_resolution_clock::now(); 
        setup.mp[randKeyVal.first] = randKeyVal.second;
        auto end = high_resolution_clock::now(); 
        
        duration<double, micro> elapsed = end-start;
        totalTime+=elapsed.count();
    }

    return totalTime;
}

double unorderedMapRandomOp(UnorderedSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times*10; i++){
        //operation to perform
        int op = rand(1,3);
        //random index of all inserted elements
        int indx = rand(0,setup.inserted.size()-1);
        auto inserted = setup.inserted[indx];
        //random to insert or search for
        auto randKeyVal = randPairs(1)[0];

        auto start = high_resolution_clock::now(); 
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
        auto end = high_resolution_clock::now();   

        //we need to keep the size consistent in our testing. This also avoids seg fault because of deletes
        if (op == 3){
            setup.mp[inserted.first] = inserted.second;
        }
        if (op == 1){
            setup.mp.erase(randKeyVal.first);
        }

        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
    }

    //return average time
    return totalTime / times;
}
double mapRandomOp(MapSetup setup){
    int times = setup.size;
    double totalTime = 0;

    for (int i=0;i < times*10; i++){
        //operation to perform
        int op = rand(1,3);
        //random index of all inserted elements
        int indx = rand(0,setup.inserted.size()-1);
        auto inserted = setup.inserted[indx];
        //random to insert or search for
        auto randKeyVal = randPairs(1)[0];

        auto start = high_resolution_clock::now(); 
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
        auto end = high_resolution_clock::now();   

        //we need to keep the size consistent in our testing. This also avoids seg fault because of deletes
        if (op == 3){
            setup.mp[inserted.first] = inserted.second;
        }
        if (op == 1){
            setup.mp.erase(inserted.first);
        }

        duration<double, micro> elapsed = end-start;
        totalTime += elapsed.count();
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

    file << hash_map << "," << measure << "," << size << "," << time << "\n";
}


int main() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream mapFile("./benchmarks/map.csv");
    ofstream unorderedFile("./benchmarks/unordered_map.csv");

    mapFile << "Type,Measure,Size,Time\n";
    unorderedFile << "Type,Measure,Size,Time\n";

    for (int i=10000; i<=300000; i+=10000){
        sizes.push_back(i);
    }
    for (int i=300000; i<=1000000; i+=100000){
        sizes.push_back(i);
    }
    for (int i=1000000; i<=3000000; i+=500000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        vector<thread> threads;

        //UnorderedMap
        threads.push_back(thread([=, &unorderedFile]() {
            benchmarkAndLog("UnorderedMap","AllRandom", size, ref(unorderedFile), unorderedMapRandomOp, setupUnorderedMap);
        }));
        threads.push_back(  thread([=, &unorderedFile]() {
            benchmarkAndLog("UnorderedMap","Insert", size, ref(unorderedFile), unorderedMapInsert, setupUnorderedMap);
        }));
        threads.push_back(  thread([=, &unorderedFile]() {
            benchmarkAndLog("UnorderedMap","Search", size, ref(unorderedFile), unorderedMapSearch, setupUnorderedMap);
        }));
        threads.push_back(  thread([=, &unorderedFile]() {
            benchmarkAndLog("UnorderedMap","Remove", size, ref(unorderedFile), unorderedMapRemove, setupUnorderedMap);
        }));

        //Map
        threads.push_back(  thread([=, &mapFile]() {
            benchmarkAndLog("Map","AllRandom", size, ref(mapFile), mapRandomOp, setupMap);
        }));
        threads.push_back(  thread([=, &mapFile]() {
            benchmarkAndLog("Map","Insert", size, ref(mapFile), mapInsert, setupMap);
        }));
        threads.push_back(  thread([=, &mapFile]() {
            benchmarkAndLog("Map","Search", size, ref(mapFile), mapSearch, setupMap);
        }));
        threads.push_back(  thread([=, &mapFile]() {
            benchmarkAndLog("Map","Remove", size, ref(mapFile), mapRemove, setupMap);
        }));

        for (auto& th : threads) {
            th.join();
        }
        cout << "done: " << size << endl;
    }

    mapFile.close();
    unorderedFile.close();

    return 0;
}

