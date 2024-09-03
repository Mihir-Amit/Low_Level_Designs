#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

class Vehicle{
    public:
    string registration_number;
    string driver_name;
    bool disability = false;
    bool pregnancy = false;
    string type;
    string model;

    Vehicle(string type, string model, string registration, string driver_name)
    {
        this->type = type;
        this->model = model;
        this->registration_number = registration;
        this->driver_name = driver_name;
    }

    void mark_disability(){
        this->disability = true;
    }
    void mark_pregnancy(){
        this->pregnancy = true;
    }
};

class Slot{
    public:
    string slot_type;
    bool is_vacant;
    int level;
    int number;

    Slot(string type, int level, int number)
    {
        this->slot_type = type;
        this->is_vacant = true;
        this->level = level;
        this->number = number;
    }
};

class Level{
    private:
    queue<Slot*> suv_slots;
    queue<Slot*> scooter_slots;
    queue<Slot*> sedan_slots;
    queue<Slot*> bike_slots;
    queue<Slot*> pregnant_slots;
    queue<Slot*> disabled_slots;

    public:
    int number;
    vector<queue<Slot*>> list;
    Level(int number){
        this->number = number;
        list.push_back(pregnant_slots);
        list.push_back(disabled_slots);
        list.push_back(suv_slots);
        list.push_back(scooter_slots);
        list.push_back(sedan_slots);
        list.push_back(bike_slots);
    }

    void add_slot(Slot* slot)
    {
        if(slot->slot_type == "Pregnant")
            list[0].push(slot);
        else if(slot->slot_type == "Disabled")
            list[1].push(slot);
        else if(slot->slot_type == "SUV")
            list[2].push(slot);
        else if(slot->slot_type == "Scooter")
            list[3].push(slot);
        else if(slot->slot_type == "Sedan")
            list[4].push(slot);
        else if(slot->slot_type == "Bike")
            list[5].push(slot);
    }
}; 

class Ticket{
    public:
    int ticket_number;
    Slot* slot;
    Vehicle* vehicle;
    time_point<high_resolution_clock> start_time;

    Ticket(int tkt, Slot* slot, Vehicle* vehicle)
    {
        this->ticket_number = tkt;
        this->slot = slot;
        this->vehicle = vehicle;
        this->start_time = high_resolution_clock::now();
    }

    time_point<high_resolution_clock> get_booking_time()
    {
        return start_time;
    }
};

class Parking_Lot{
    private:
    vector<Level*> levels;
    map<int, Ticket*> ticket_list;
    map<string, int> mapping;
    static Parking_Lot* parking_lot;
    int ticket_counter;
    int cps;
    Parking_Lot(){
        cout<<"Parking Lot Created"<<endl;
        mapping["Pregnant"] = 0;
        mapping["Disabled"] = 1;
        mapping["SUV"] = 2;
        mapping["Scooter"] = 3;
        mapping["Sedan"] = 4;
        mapping["Bike"] = 5;
        cps = 5;
        ticket_counter = 0;
    }

    public:
    static Parking_Lot* instantiate(){
        if(parking_lot == nullptr)
            parking_lot = new Parking_Lot();
        return parking_lot;
    }
    void add_level(Level* level)
    {
        this->levels.push_back(level);
    }   

    int create_ticket(Vehicle* vehicle){
        string vehicle_type = vehicle->type;
        int index = mapping[vehicle_type];
        bool ticket_assigned = false;
        Ticket* ticket;
        for(int i=0; i<levels.size(); i++)
        {
            // cout<<levels[i]->number<<endl;
            // cout<<index<<endl;
            if(levels[i]->list[index].size() > 0)
            {
                ticket_assigned = true;
                ticket = new Ticket(ticket_counter, levels[i]->list[index].front(), vehicle);
                levels[i]->list[index].pop();
                ticket_counter++;
                break;
            }
        }
        if(ticket_assigned)
        {
            ticket_list[ticket->ticket_number] = ticket;
            return ticket->ticket_number;
        }
        else
            return -1;
    }

    double exit(int ticket_id){
        auto ticket = ticket_list[ticket_id];
        auto start = ticket->get_booking_time();
        auto end = high_resolution_clock::now();
        duration<double> time_parked = end - start;
        ticket_list.erase(ticket_id);
        levels[ticket->slot->level-1]->list[mapping[ticket->slot->slot_type]].push(ticket->slot);
        // cout<<"noice"<<endl;
        // cout<<time_parked.count()<<endl;
        return time_parked.count()*cps;
    }
};

Parking_Lot* Parking_Lot::parking_lot = nullptr;

class Interface{
    private:
    Parking_Lot* parking_lot;
    mutex mtx;

    public:
    Interface(){
        parking_lot = Parking_Lot::instantiate();
    }

    void add_level(Level* level)
    {
        parking_lot->add_level(level);
    }

    int entry(Vehicle* vehicle){
        lock_guard<mutex> lock(mtx);
        return parking_lot->create_ticket(vehicle);
    }

    double exit(int ticket_id){
        lock_guard<mutex> lock(mtx);
        return parking_lot->exit(ticket_id);
    }
};


int main()
{
    Interface* ifc = new Interface();
    Level* level1 = new Level(1);
    Slot* slot1 = new Slot("SUV", 1, 1);
    Slot* slot2 = new Slot("SUV", 1, 2);
    Slot* slot3 = new Slot("Bike", 1, 3);
    level1->add_slot(slot1);
    level1->add_slot(slot2);
    level1->add_slot(slot3);
    ifc->add_level(level1);

    Vehicle* vehicle1 = new Vehicle("SUV", "honda", "12345", "mihir");
    Vehicle* vehicle2 = new Vehicle("SUV", "bmw", "12345", "mihir");
    Vehicle* vehicle3 = new Vehicle("SUV", "merc", "1223", "mihir");

    cout<<ifc->entry(vehicle1)<<endl;
    cout<<ifc->entry(vehicle2)<<endl;
    cout<<ifc->entry(vehicle3)<<endl;
    cout<<ifc->exit(1)<<endl;
    cout<<ifc->entry(vehicle3)<<endl;
}