#include <omp.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>
#include <stack>
#include <string>
#include <valarray>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

// Function to calculate maximum overlap in two given strings
int findOverlappingPair(string str1, string str2, string& str)
{
    // max will store maximum overlap i.e maximum
    // length of the matching prefix and suffix
    int max = INT_MIN;
    int len1 = str1.length();
    int len2 = str2.length();

    // check suffix of str1 matches with prefix of str2
    for (int i = 1; i <= min(len1, len2); i++)
    {
        // compare last i characters in str1 with first i
        // characters in str2
        if (str1.compare(len1 - i, i, str2, 0, i) == 0)
        {
            if (max < i)
            {
                //update max and str
                max = i;
                str = str1 + str2.substr(i);
            }
        }
    }

    // check prefix of str1 matches with suffix of str2
    for (int i = 1; i <= min(len1, len2); i++)
    {
        // compare first i characters in str1 with last i
        // characters in str2
        if (str1.compare(0, i, str2, len2 - i, i) == 0)
        {
            if (max < i)
            {
                //update max and str
                max = i;
                str = str2 + str1.substr(i);
            }
        }
    }
    return max;
}

// Function to calculate smallest string that contains
// each string in the given set as substring.
void findShortestSuperstring(int thread_num, vector<string> arr, int a)
{
    int len = arr.size();
    int start;
    if (len != a) {
        start = len/2;
    }
    else {
        start = len;
    }

    // run len-1 times to consider every pair
    while (len != 1)
    {
        // to store  maximum overlap
        int max = INT_MIN;

        // to store array index of strings
        // involved in maximum overlap
        int l, r;

        // to store resultant string after maximum overlap
        string resStr;

        for (int i = 0; i < start; i++)
        {
            for (int j = i + 1; j < len; j++)
            {
                string str;

                // res will store maximum length of the matching
                // prefix and suffix str is passed by reference and
                // will store the resultant string after maximum
                // overlap of arr[i] and arr[j], if any.
                int res = findOverlappingPair(arr[i], arr[j], str);

                // check for maximum overlap
                if (max < res)
                {
                    max = res;
                    resStr.assign(str);
                    l = i, r = j;
                }
            }
        }

        // ignore last element in next cycle
        len--;

        // if no overlap, append arr[len] to arr[0]
        if (max == INT_MIN)
            arr[0] += arr[len];
        else
        {
            // copy resultant string to index l
            arr[l] = resStr;

            // copy string at last index to index r
            arr[r] = arr[len];
        }
    }
    cout << arr[0] << endl;
}

int main()
{
    int thread_num = 2;
    omp_set_num_threads(thread_num);
    clock_t stop_time;
    vector<string> arr = { "abc", "cde", "dew" };
    for (int i = 0; i < 100; i++) {
        arr.push_back("er");
    }
    clock_t begin_time = clock();
    int pos = 0, length = 0;
#pragma omp parallel
    findShortestSuperstring(thread_num, arr, arr.size() / thread_num);
    stop_time = clock();
    cout << "Parallel time on " << thread_num << " threads: " << double(stop_time - begin_time) / CLOCKS_PER_SEC << endl;

    thread_num = 1;
    begin_time = clock();
    pos = 0; length = 0;
    findShortestSuperstring(thread_num, arr, arr.size()/thread_num);
    stop_time = clock();

    cout << "Single time: " << double(stop_time - begin_time) / CLOCKS_PER_SEC <<  endl;

    system("pause");
    return 0;
}