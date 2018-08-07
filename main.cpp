#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#define INPUT_FILE "/home/zuzu/CLionProjects/akwb4/A.txt"

using namespace std;

vector<unsigned int> read_from_file() {
    vector<unsigned int> numbers;
    unsigned int number;
    ifstream file(INPUT_FILE);
    if (!file.good()) {
        fprintf(stderr, "Nie udało się otworzyć pliku!");
        exit(2);
    }
    while (file >> number) numbers.push_back(number);
    file.close();
    return numbers;
}

unsigned int find_cuts_no(const unsigned long instanceSize) {
    const unsigned long doubleInstanceSize = 2 * instanceSize;
    unsigned int i = 0;
    for (; i < doubleInstanceSize / 3; i++)               // Przy d=x/3 (d+1)(d+2)>x
        if ((i + 1) * (i + 2) == doubleInstanceSize)
            break;
    if (i == doubleInstanceSize / 3) {
        fprintf(stderr, "Nie znaleziono calkowitej liczby ciec, zadana instancja jest bledna.\n");
        exit(1);
    }
    return i;
}

void erase(vector<unsigned int> &v, const unsigned int n) {
    const auto it = find(v.begin(), v.end(), n);
    if (it != v.end()) v.erase(it);
}

vector<unsigned int> map(vector<unsigned int> &input, const unsigned int k) {
    vector<unsigned int> temp;
    vector<vector<unsigned int>> used;
    vector<unsigned int> result;

    auto it1 = input.end() - 1;
    auto it2 = input.end() - 2;
    result.push_back(*it1 - *it2);
    erase(input, *it1 - *it2);
    unsigned int max_sum = input.back();
    for (auto candidate = input.begin(); candidate != input.end();) {
        if (result.size() == (k + 1)) return result;
        bool sum_found = true;
        temp.clear();
        unsigned int sum = *candidate;
        bool back = false;
        for (auto res = result.rbegin(); res != result.rend(); res++) {
            sum += *res;
            unsigned int last = 0;
            if (sum > max_sum || (*candidate == max_sum - result.front()) ||
                (*candidate == input.back() && result.size() < k)) {
                last = result.back();
                result.pop_back();
                input.push_back(last);
                back = true;
                if (!used.empty()) {
                    for (auto &e : used.back())
                        input.push_back(e);
                    used.pop_back();
                }
                sort(input.begin(), input.end());
                for (auto it = input.begin() + 1; it != input.end(); it++)
                    if (*it > last) {
                        candidate = it - 1;
                        break;
                    }
                for (auto &sums : temp) input.push_back(sums);
                sort(input.begin(), input.end());
                break;
            }
            if (find(input.begin(), input.end(), sum) == input.end()) {
                sum_found = false;
                break;
            }
            temp.push_back(sum);
            erase(input, sum);
        }

        if (back) {
            candidate++;
            continue;
        }

        if (!sum_found) {
            for (auto &sums : temp) input.push_back(sums);
            sort(input.begin(), input.end());
        } else {
            result.push_back(*candidate);
            erase(input, *candidate);
            used.push_back(temp);
            candidate = input.begin();
            continue;
        }
        candidate++;
    }
}

int main() {
    vector<unsigned int> instance;
    instance = read_from_file();
    cout << "zawartosc wektora: ";
    for (auto &a: instance) cout << a << " ";
    cout << endl;
    cout << "dlugosc wektora: " << instance.size() << endl;
    unsigned int k = find_cuts_no(instance.size());
    cout << "k: " << k << endl;
    vector<unsigned int> result;
    long duration = 0;
    chrono::high_resolution_clock::time_point t1;
    chrono::high_resolution_clock::time_point t2;
    if (is_sorted(instance.begin(), instance.end())) {
        t1 = chrono::high_resolution_clock::now();
        result = map(instance, k);
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    } else if (is_sorted(instance.rbegin(), instance.rend())) {
        t1 = chrono::high_resolution_clock::now();
        vector<unsigned int> T(instance.rbegin(), instance.rend());
        result = map(T, k);
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    } else {
        cout << "Nieprawidłowa instanja" << endl;
    }
    if (result.size() == k + 1) {
        cout << "długość rozwiązania: " << result.size() << endl << "Mapa: ";
        for (auto &r: result) cout << r << " ";
        cout << endl;
        cout << "Obliczenia trwały: " << duration << " millisekund" << endl;
    } else {
        cout << "Nie znaleziono prawidłowego rozwiązania" << endl;
    }
    return 0;
}