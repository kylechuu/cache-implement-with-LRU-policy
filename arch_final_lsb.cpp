#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<vector>
#include<math.h>
#include<queue>
using namespace std;
class LSB{
        public:
            LSB();
            void GetBits();
            void ReadAndWrite_TheFiles(string inputOne, string inputTwo, string output);
        private:
            double Sets;
            double Way;
            double BitsOffset;
            double BusBits;
            double Index;
            double BlockSize;
            double Tags;
            int missCount;
            vector<vector<string> > cacheVector;
            queue <double> IndexingBits;

};
int main(int argc, char const *argv[]){
    string input_one, input_two, output;
    input_one.assign(argv[1]);
    input_two.assign(argv[2]);
    output.assign(argv[3]);
    LSB CacheMethod;
    CacheMethod.ReadAndWrite_TheFiles(input_one, input_two, output);
    return 0;
}
    LSB::LSB(){ 
        Sets = 0;
        Way = 0;
        BitsOffset = 0;
        BusBits = 0;
        Index = 0;
        BlockSize = 0;
        Tags = 0;
        missCount = 0;
        cacheVector.resize(1000000);
        //IndexingBits.resize(15);
    }   
void LSB::GetBits(){
    Index = log(Sets)/log(2);  //number of index
    //cout<<BlockSize<<"\n";
    BitsOffset = log(BlockSize)/log(2); //number of offset
    //cout<<BitsOffset<<"\n";
    //cout<<log(64)/log(2)<<"\n";
    Tags = BusBits - Index - BitsOffset; //number of tags
    double temp = BusBits - Tags -1;
    for(int i=0;i<Index;i++){
        IndexingBits.push(temp);
        temp --;
    }
}
void LSB::ReadAndWrite_TheFiles(string inputOne, string inputTwo, string output){
    //string inputpathOne, inputpathTwo, outputpath;
    string whatever, str, tagStr, indexStr, binaryBits;
    ifstream cacheInfo(inputOne);
    ifstream reference(inputTwo);
    ofstream outputfile(output);
    if(outputfile.is_open())
        if(reference.is_open())
            if(cacheInfo.is_open()){
                cacheInfo>>whatever; cacheInfo>>BusBits;
                outputfile<<"Address bits: "; outputfile<<BusBits<<"\n";
                cacheInfo>>whatever; cacheInfo>>Sets;
                outputfile<<"Number of sets: "; outputfile<<Sets<<"\n";
                cacheInfo>>whatever; cacheInfo>>Way;
                outputfile<<"Associativity: "; outputfile<<Way<<"\n";
                cacheInfo>>whatever; cacheInfo>>BlockSize;
                outputfile<<"Block size: "; outputfile<<BlockSize<<"\n";
                outputfile<<"\n";
                GetBits();
                outputfile<<"Indexing bit count: "; outputfile<<Index<<"\n";
                outputfile<<"Indexing bits: "; 
                while(!IndexingBits.empty()){
                    outputfile<<IndexingBits.front()<<" ";
                    IndexingBits.pop();
                }
                outputfile<<"\n";
                outputfile<<"Offset bit count: "; outputfile<<BitsOffset<<"\n";
                outputfile<<"\n";
                reference>>whatever; outputfile<<whatever<<" "; 
                reference>>whatever; outputfile<<whatever<<"\n";
                while(reference>>str){
                    if(str==".end") break ; 
                    else{
                        outputfile<<str<<" ";
                        int currentIndex = Index , i = 0 ,allocation = 0;
                        tagStr = str.substr(0,Tags);
                        indexStr = str.substr(Tags,Index);
                        for(int j = 0;j < Index; j++){
                            binaryBits = indexStr.substr(i,1);
                            if(binaryBits == "1") allocation += pow(2,(currentIndex-1));  
                            currentIndex --;
                            i++ ;
                        }
                        //cout<<"tagStr: "<<tagStr<<" "<<"indexStr: "<<indexStr<<" "<<"allocation: "<<allocation<<"\n";
                        if(cacheVector[allocation].size() != 0){
                            for(int j = 0; j< cacheVector[allocation].size(); j++){
                                if(cacheVector[allocation][j] == tagStr){ //hit
                                    for(vector<string>::iterator iter=cacheVector[allocation].begin(); iter!=cacheVector[allocation].end();){
                                        if(*iter == tagStr)  iter = cacheVector[allocation].erase(iter);
                                        else iter++;
                                    }
                                    cacheVector[allocation].push_back(tagStr);
                                    outputfile<<"hit\n"; 
                                    break;
                                }
                                else if(j == cacheVector[allocation].size()-1){   //miss
                                    if(cacheVector[allocation].size()+1 <= Way ){
                                        cacheVector[allocation].push_back(tagStr);
                                        missCount++;
                                        outputfile<<"miss\n";
                                        break;
                                    }
                                    else{
                                        vector<string>::iterator itr = cacheVector[allocation].begin();  //miss
                                        cacheVector[allocation].erase(itr);
                                        cacheVector[allocation].push_back(tagStr);
                                        missCount++;
                                        outputfile<<"miss\n";
                                        break;
                                    }
                                } 
                            }
                        }
                        else{
                            cacheVector[allocation].push_back(tagStr); //miss
                            missCount++;
                            outputfile<<"miss\n";
                        }
                        // for(int i=0;i<cacheVector[allocation].size();i++) 
                        //     cout<<cacheVector[allocation][i]<<" ";
                        // cout<<"\n";   
                    }
                }
                outputfile<<".end"<<"\n";
                outputfile<<"\n";
                outputfile<<"\n";
                outputfile<<"Total cache miss count: "<<missCount;
            }
    cacheInfo.close();
    reference.close();
    outputfile.close();
}
