#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
string randString(size_t length) {
    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(0, chars.size() - 1);

    string res;
    for (size_t i = 0; i < length; ++i) {
        res += chars[distribution(generator)];
    }

    return res;
}

vector<pair<string,int>> randPairs(int amount){
    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(-1000000, 1000000);
    vector<pair<string,int>> res;

    for (int i=0; i<amount; i++)
        res.push_back(make_pair(randString(15),distribution(generator)));

    return res;
}

struct UnorderedSetup{
    unordered_map<string,int> mp;
    int size;

    UnorderedSetup(unordered_map<string,int> mp, int size) : mp(mp), size(size) {}
};

UnorderedSetup setupUnorderedMap(int size){
    unordered_map<string,int> mp;
    auto pairs = randPairs(size);

    for (auto x:pairs)
        mp[x.first] = x.second;

    return UnorderedSetup(mp,size);
}

struct MapSetup{
    map<string,int> mp;
    int size;

    MapSetup(map<string,int> mp, int size) : mp(mp), size(size) {}
};

MapSetup setupMap(int size){
    map<string,int> mp;
    auto pairs = randPairs(size);

    for (auto x:pairs)
        mp[x.first] = x.second;

    return MapSetup(mp,size);
}

int benchmarkUnorderedMap(UnorderedSetup setup){
    return 0;
}

int benchmarkMap(MapSetup setup){
    return 0;
}

template <typename Func,typename Setup>
void benchmarkAndLog(
    const string& hash_map,
    int size,
    ofstream& file,
    Func operation,
    Setup setup
){
    double time = operation(setup(size));

    cout << "done: " << hash_map << "|" << size << endl;
    file << hash_map << "," << size << "," << time << "\n";
}


int main() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream mapFile("./benchmarks/map.csv");
    ofstream unorderedFile("./benchmarks/unordered_map.csv");

    mapFile << "Type,Size,Time\n";
    unorderedFile << "Type,Size,Time\n";

    for (int size : sizes) {
        thread threads[2];

        threads[0] =  thread([=, &mapFile]() {
            benchmarkAndLog("Map", size, ref(mapFile), benchmarkMap, setupMap);
        });

        threads[1] =  thread([=, &unorderedFile]() {
            benchmarkAndLog("Map", size, ref(unorderedFile), benchmarkUnorderedMap, setupUnorderedMap);
        });

        for (auto& th : threads) {
            th.join();
        }
    }

    mapFile.close();
    unorderedFile.close();

    return 0;
}

