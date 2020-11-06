//
// Created by kszlzj on 2020/11/4.
//

#ifndef BANKERS_ALGORITHM_BANKER_HPP
#define BANKERS_ALGORITHM_BANKER_HPP
#include "vector"
#include "map"
#include "iostream"
class Banker{
private:
    std::vector<int> available; /*剩余空闲量*/
    std::vector<std::vector<int> > Max;/*总需求*/
    std::vector<std::vector<int> > allocation;/*已分配量*/
    std::vector<std::vector<int> > need;/*未来需要量*/
    /* Need[i,j] = Max[i,j] - Allocation[i,j] */
    int thread;
    int needs;
    bool current_satisfication_check(std::vector<int> & work,std::vector<bool> & finish){
        bool flag=false;
        for(int i=0;i<thread;i++){
            if(finish[i]== false){
                for(int j=0;j<need[i].size();j++){
                    if(need[j][i]<=work[i]){
                        flag = true;
                    }
                }
            }
        }
        if(!flag){
            return false;
        }
        return true;
    }
public:
    Banker(std::vector<int>&available,std::vector<std::vector<int> >& Max,std::vector<std::vector<int> >&allocation ,std::vector<std::vector<int> >&need){
        this->available=available;
        this->Max=Max;
        this->allocation=allocation;
        this->need=need;
        thread=Max.size();
        needs=available.size();
    }
    bool security_check(){
        std::vector<int> work(available.begin(),available.end());
        std::vector<bool> finish(thread,false);
AVAILABLE_CHECK:
        if(!current_satisfication_check(work,finish)){
            goto FINAL_CHECK;
        }
        for(int i=0;i<allocation.size();i++){
            for(int j=0;j<allocation[i].size();j++){
                work[j]+=allocation[i][j];
                finish[i]= true;
                goto AVAILABLE_CHECK;
            }
        }
FINAL_CHECK:
        for(int i=0;i<finish.size();i++){
            if(!finish[i])return false;
        }
        return true;
    }
    bool request(std::vector<int>& requests , int pid){
        std::vector<int> available_copy(available.begin(),available.end());
        std::vector<std::vector<int> > Max_copy(Max.begin(),Max.end());
        std::vector<std::vector<int> > allocation_copy(allocation.begin(),allocation.end());
        std::vector<std::vector<int> > need_copy(need.begin(),need.end());
        for(int i=0;i<requests.size();i++){
            if(requests[i]>need[pid][i]){
                std::cout<<"BAD REQUEST! : It has exceeded the needs"<<std::endl;
                exit(0);
            }
        }
        for(int i=0;i<requests.size();i++){
            if(requests[i]>available[i]){
                std::cout<<"BAD REQUEST! : The system cannot provide enough resources"<<std::endl;
                exit(0);
            }
        }
        for(int i=0;i<requests.size();i++){
            available[i]-=requests[i];
            allocation[pid][i]+=requests[i];
            need[pid][i]-=requests[i];
        }
        if(!security_check()){
            available=available_copy;
            Max=Max_copy;
            allocation=allocation_copy;
            need=need_copy;
        }
    }
};


#endif //BANKERS_ALGORITHM_BANKER_HPP
