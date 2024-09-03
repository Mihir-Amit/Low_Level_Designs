#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Person{
    private:
    string name;
    public:
    int id;
    Person(int id, string name)
    {
        this->id = id;
        this->name = name;
    }
};
class Request{
    private:
    Person* requester;
    int request_id;
    public:
    time_point<high_resolution_clock> request_time;
    string request_string;
    Request(string request, Person* requester, int id)
    {
        this->request_string = request;
        this->requester = requester;
        this->request_id = id;
        this->request_time = high_resolution_clock::now();
    }
};
class RequestHandler{
    private:
    map<Person*, queue<Request*>> request_map;
    mutex mtx;
    static RequestHandler* ptr;
    RequestHandler(){}

    public:
    static RequestHandler* instantiate()
    {
        if(ptr == nullptr)
        {
            ptr = new RequestHandler();
        }
        return ptr;
    }

    void RequestHandling(Person* requester, Request* request)
    {
        lock_guard<mutex> lock(mtx);
        if(request_map.find(requester)==request_map.end())
        {
            queue<Request*> request_hitory;
            request_hitory.push(request);
            request_map[requester] = request_hitory;
            cout<<request->request_string<<endl;
        }
        else
        {
            // auto request_map[requester] = request_map[requester];
            while(!request_map[requester].empty())
            {
                auto time_now = high_resolution_clock::now();
                duration<double> elapsed = time_now - request_map[requester].front()->request_time;
                cout<<elapsed.count()<<endl;
                if(elapsed.count() >=1.0 )
                    request_map[requester].pop();
                else
                    break;
            }
            if(request_map[requester].size() >= 10)
            {
                cout<<"Request "<<request->request_string<<" failed"<<endl;
            } 
            else
            {
                cout<<"Request "<<request->request_string<<" completed"<<endl;
                request_map[requester].push(request);
            }
        }
    }
};

RequestHandler* RequestHandler::ptr = nullptr;

int main()
{
    Person* mihir = new Person(0, "mihir");
    RequestHandler* request_db = RequestHandler::instantiate();
    for(int i = 0; i<12; i++)
    {
        string str = "" + to_string(i);
        Request* request = new Request("Request " + str, mihir, i);
        request_db->RequestHandling(mihir, request);
    }
    this_thread::sleep_for(chrono::seconds(2));
    Request* request = new Request("Request final", mihir, 13);
    request_db->RequestHandling(mihir, request);
}
