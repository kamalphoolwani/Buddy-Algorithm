#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

class Solution {
public:
    void make_row_zero(unordered_set<string> &cells, int row, int size){
        for(int col = 0 ; col < size ; col++){
            cells.insert(to_string(row)+" "+to_string(col));
        }
    }
    void make_col_zero(unordered_set<string> &cells, int col, int size){
        for(int row = 0 ; row < size ; row++){
            cells.insert(to_string(row)+" "+to_string(col));
        }
    }
    // void print(vector<vector<int>>& matrix){
    //     for(auto row:matrix){
    //         for(auto col:row){
    //             cout<<col<<" ";
    //         }
    //         cout<<endl;
    //     }
    // }
    void setZeroes(vector<vector<int>>& matrix) {
        unordered_set<string> cells;
        for(int i = 0 ; i < matrix.size() ; i++){
            for(int j = 0 ; j < matrix[0].size() ; j++){
                if(matrix[i][j] == 0){
                    make_row_zero(cells, i, matrix[0].size());
                    make_col_zero(cells, j, matrix.size());
                }                
            }
            // print(matrix);cout<<endl;
        }
        for(auto p : cells){
            int row = stoi(p.substr(0,p.find(' ')));
            int col = stoi(p.substr(p.find(' ')));
            matrix[row][col] = 0;
        }
    }
};