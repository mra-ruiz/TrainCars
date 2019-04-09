// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//
// =======================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include "traincar.h"
#include "traincar_prototypes.h"

// =======================================================================
// =======================================================================
//
//  HERE'S WHERE YOU SHOULD IMPLEMENT THE FUNCTIONS YOU PROTOTYPED IN
//  THE "traincar_prototypes.h" FILE.
//

//============================FOR SimpleTrainTest() FUNCTION===================

//adds a car to the end of the list
void PushBack(TrainCar*& first_car, TrainCar* car) {
    //when there is nothing in the list, make the list point to the car
    if (first_car==NULL) {
        first_car=car;
    }
    else{
        //temporary pointer helps us find the last car in the list
        TrainCar* last_car = first_car;
        while (last_car->next!=NULL) {
            last_car=last_car->next;
        }
        car->prev=last_car;
        last_car->next=car;
    }
}

//deletes everything in the list making sure we don't have memory leaks
void DeleteAllCars(TrainCar*& pointer){
    //if the pointer IS null then nothing happens. otherwise....
    if (pointer != NULL) {
        while (pointer->next != NULL) {
            pointer = pointer->next;
            delete pointer->prev;
        }
        delete pointer;
        pointer = NULL;
    }
}

//=============================HELPER FUNCTIONS================================

//removes first car from front of train but it stores it as a TrainCar* pointer
TrainCar* PopFront(TrainCar*& first_car) {
    //if my list is null then return null because we can't pop anything
    if (first_car==NULL) {
        return NULL;
    }
    TrainCar* popped = first_car;
    first_car = first_car->next;
    if (first_car!=NULL) {
        first_car->prev=NULL;
    }
    popped->prev=NULL;
    popped->next=NULL;
    return popped;
}

//removes last car from back of train but it stores it as a TrainCar* pointer
TrainCar* PopBack(TrainCar*& last_car){
    //if my list is null then return null because we can't pop anything
    if (last_car==NULL) {
        return NULL;
    }
    TrainCar* popped = last_car;
    while (popped->next!=NULL) {
        popped=popped->next;
    }
    if (popped->prev!=NULL) {
        popped->prev->next=NULL;
    }
    popped->prev=NULL;
    popped->next=NULL;
    return popped;
}

//adds a car to front of the train
void PushFront(TrainCar*& first_car, TrainCar* car) {
    //when there is nothing in the list, make the list point to the car
    if (first_car==NULL) {
        first_car=car;
    }
    else if (first_car != NULL){
        first_car->prev = car;
        if (car!= NULL) {
            car->next = first_car;
            car->prev=NULL;
            first_car = car;
        }
    }
}

//just removes a car from anywhere between the beginnig and end of a list and
//stores it as a TrainCar* pointer
TrainCar* Pop(TrainCar*& car){
    //if the car is empty then return null because we can't pop a null
    if (car==NULL) {
        return NULL;
    }
    TrainCar* popped = car;
    if (popped->prev!=NULL) {
        popped->prev->next = car->next;
    }
    if (popped->next!=NULL) {
        popped->next->prev = car->prev;
    }
    popped->prev=NULL;
    popped->next=NULL;
    return popped;
}

//unlike PushFront, this inserts a car anywhere in the list prior to the
//specified locatin. So basically this work if I want to insert a car somewhere
//between the beginning and end of the list
void InsertBefore(TrainCar*& middle, TrainCar* to_insert){
    //if there is nothing to insert then return nothing
    if (to_insert == NULL) {
        return;
    }
    //if there isn't anything in the list then return null
    if (middle==NULL) {
        middle=to_insert;
    }
    else {
        if (middle->prev != NULL) {
            middle->prev->next = to_insert;
        }
        to_insert->prev = middle->prev;
        middle->prev = to_insert;
        to_insert->next = middle;
    }
}

//unlike PushBack, this inserts a car anywhere in the list after the specified
//locatin. So basically this work if I want to insert a car somewhere
//between the beginning and end of the list
void InsertAfter(TrainCar*& middle, TrainCar* to_insert){
    //if there is nothing to insert then return nothing
    if (to_insert == NULL) {
        return;
    }
    //if there isn't anything in the list then return null
    if (middle==NULL) {
        middle=to_insert;
    }
    else {
        if (middle->next != NULL) {
            middle->next->prev = to_insert;
        }
        to_insert->next = middle->next;
        middle->next = to_insert;
        to_insert->prev = middle;
    }
}

//===============FOR ShipFreightHelper() FUNCTION==============================
void TotalWeightAndCountCars(TrainCar* car, int& weight, int& num_engines,
    int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars,
    int& num_sleeping_cars) {
    //setting all passed in variables to 0
    weight = num_engines = num_freight_cars = num_passenger_cars = 0;
    num_dining_cars = num_sleeping_cars = 0;
    while (car != NULL) {
        weight += car->getWeight();
        if (car->isEngine()) {
            num_engines++;
        }
        else if (car->isFreightCar()) {
            num_freight_cars++;
        }
        else if (car->isPassengerCar()) {
            num_passenger_cars++;
        }
        else if (car->isDiningCar()) {
            num_dining_cars++;
        }
        else if (car->isSleepingCar()) {
            num_sleeping_cars++;
        }
        car = car->next;
    }
}

float CalculateSpeed(TrainCar* car) {
    int total_weight,num_engines,num_freight_cars,num_passenger_cars;
    int num_dining_cars,num_sleeping_cars;
    TotalWeightAndCountCars(car,total_weight,num_engines,num_freight_cars,
        num_passenger_cars,num_dining_cars,num_sleeping_cars);
    float top = ((float)num_engines*3000)*550*3600;
    float bottom = 20*2*5280* (float)total_weight;
    float speed = top/bottom;
    return speed;
}

/*
makes vector with the fewest number of trains necessary to ship all freights.
Each train pointer contains a train with the fewest number of engines.
*/
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights,
    int min_speed, int max_cars) {

    //making the vector for all the train combinations
    std::vector<TrainCar*> train_options;

    //runs until there are no more train options
    while (engines != NULL && freights != NULL) {
        //keeps track of when we add cars and when speed changes
        //also keeps track our last car added to our current train
        int num_cars = 0;
        float current_speed = 0;
        TrainCar* current_train = NULL;

        //add an engine first no matter what
        TrainCar* engine_to_add = PopFront(engines);
        PushBack(current_train, engine_to_add);
        num_cars++;
        current_speed = CalculateSpeed(current_train);

        //keeps adding freights to train (current_train pointer) as long as
        //the requirements for maximum number of cars per train and
        //minimum speed per train are met and there are still freights to add.
        //Each loop iteration adds one car to the current train
        while (num_cars < max_cars && freights != NULL) {
            TrainCar* freight_to_add = PopFront(freights);
            PushBack(current_train, freight_to_add);
            num_cars++;
            current_speed = CalculateSpeed(current_train);
        }

        //after we add all the cars, we calculate speed to see if its too slow
        current_speed = CalculateSpeed(current_train);

        //Here we remove freights while we are still too slow and if even after
        //removing freights we are STILL too slow, then add an engine. This way
        //engines are added only if absolutely necessary
        while(current_speed < min_speed && engines != NULL) {
            TrainCar* freight = PopBack(current_train);
            PushFront(freights, freight);
            current_speed = CalculateSpeed(current_train);
            if (current_speed < min_speed) {
                TrainCar* engine = PopFront(engines);
                PushFront(current_train, engine);
            }
            //updates speed every time to prevent infinite loop
            current_speed = CalculateSpeed(current_train);
        }

        //In the case where we have no engines left and we are still too slow,
        //then we just remove freights
        if (engines == NULL) {
            while (current_speed < min_speed) {
                TrainCar* freight = PopBack(current_train);
                PushFront(freights, freight);
                current_speed = CalculateSpeed(current_train);
            }
        }

        //adding the train to the vector if the first car in the train is an
        //engine. This prevents trains created without engines
        if (current_train->isEngine())
            train_options.push_back(current_train);
    }

    return train_options;
}

//========================FOR Separate() FUNCTION==============================
/*
Splits original train into two smaller trains where each have an engine and
the number of unlinks, link and shifts are minimized to make this more
cost efficient.
*/
void Separate(TrainCar*&original, TrainCar*& separated1, TrainCar*& separated2){
    //not all values are used in separete, but its nice to use the helper
    //function since we have it
    int total_weight,num_engines,num_freight_cars,num_passenger_cars;
    int num_dining_cars,num_sleeping_cars;
    TotalWeightAndCountCars(original, total_weight, num_engines,
        num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars);

    //Calculate the length of our original train
    int non_engines = num_passenger_cars + num_dining_cars+ num_sleeping_cars;
    int length = num_engines + non_engines;

    //checks to make sure function stops if there is only 1 engines or less
    if (num_engines < 2) { return; }

    //counter to keep track where the first engine in our train is. It starts
    //at -1 because technically 0 is a valid position in the list
    int engine1_location = -1;
    int engine2_location = -1;

    //points at the first engine found in our train
    TrainCar* engine_in_train = original;

    //finds location of both engines
    for (int i = 0; i < length; i++){
        if (engine_in_train->isEngine()) {
            //if statement runs only when we already have engine 1's location
            //so basically location of engine 2 is defined here
            if (engine1_location != -1) {
                engine2_location = i;
                break;
            }
            engine1_location = i;
        }
        //location of engine 1 is defined here
        engine_in_train=engine_in_train->next;
    }

    //find the midpoint of the list. if the midpoint is odd and the first
    //engine is at that midpoint, then move the midpoint over one so that the
    //first engine stays with the first separated train (separated1)
    int midpoint = length/2;
    if (length%2 !=0 && engine1_location == midpoint) {
        midpoint = engine1_location +1;
    }

    //checks if engine 1 is in the first half of the train
    bool eng1_in_first_half = (engine1_location < midpoint);

    //checks if engine 2 is in the second half of the train
    bool eng2_in_second_half = (engine2_location >= midpoint);

    //separate the original train using passed in pointers if there is an
    //engine in both halves of the original train
    separated1 = original;
    separated2 = original;
    for (int i = 0; i < midpoint; i++) {
        separated2 = separated2->next;
    }

    //if engine 1 is in the second half of the train then remove it from there,
    //and insert before the second train pointer. If the length is odd, before
    //inserting engine 1 before the division point, move second train pointer
    //back 1
    if (!eng1_in_first_half){
        TrainCar* find_engine_1 = original;
        for (int i = 0; i < engine1_location; i++) {
            find_engine_1 = find_engine_1->next;
        }
        TrainCar* eng1_to_move = Pop(find_engine_1);
        if (length% 2 == 0) {
            separated2 = separated2->prev;
        }
        InsertBefore(separated2, eng1_to_move);

    }

    //if engine 2 is in the first half of the train then remove it from there,
    //and insert after the second train pointer. Then, move second train
    //pointer next 1 so it points at engine 2
    else if (!eng2_in_second_half){
        TrainCar* find_engine_2 = original;
        for (int i = 0; i < engine2_location; i++) {
            find_engine_2 = find_engine_2->next;
        }
        TrainCar* eng2_to_move = Pop(find_engine_2);
        InsertAfter(separated2, eng2_to_move);
        separated2 = separated2->next;

    }

    //Here is where we actually split the trains
    separated2->prev->next = NULL;
    separated2->prev = NULL;
    original = NULL;
}

//=====================FOR PrintTrain() FUNCTION==============================
/*
This is the extra credit. I didn't do it but I made these function so that
there would be no compiler errors.
*/

float AverageDistanceToDiningCar(TrainCar* train){
    float thing = 0.0;
    return thing;
}

int ClosestEngineToSleeperCar(TrainCar* train){
    /*
    int total_weight,num_engines,num_freight_cars,num_passenger_cars;
    int num_dining_cars,num_sleeping_cars;
    TotalWeightAndCountCars(train, total_weight, num_engines,
        num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars);

    //length of the entire train
    int length=num_engines+num_passenger_cars+num_dining_cars+num_sleeping_cars;

    //Create pointers for each sleeper (if applicable)
    bool sleeper2bool = false;
    TrainCar* sleeper1 = train;
    if (num_sleeping_cars > 1) {
        TrainCar* sleeper2 = train;
        sleeper2bool = true;
    }

    //Create pointers for each engine (if applicable)
    bool engine2bool = false;
    TrainCar* engine1 = train;
    if (num_engines > 1) {
        TrainCar* engine2 = train;
        engine2bool=true;
    }

    //counter to keep track where the engines in our train are. It starts
    //at -1 because technically 0 is a valid position in the list
    int engine1_location = -1;
    if (engine2bool) {
        int engine2_location = -1;
    }

    //points at the first engine found in our train
    for (int i = 0; i < length; i++) {
        if (engine1->isEngine()) {
            engine1_location = i;
            break;
        }
        engine1=engine1->next;
    }

    if(engine2bool) {
        TrainCar* engine2 = engine1;
        //points at the second engine found in our train
        for (int i = engine1_location+1; i < length; i++) {
            if (engine2->isEngine()) {
                engine2_location = i;
                break;
            }
            engine2=engine2->next;
        }
    }

    //counter to keep track where the sleepers in our train are. It starts
    //at -1 because technically 0 is a valid position in the list
    int sleeper1_location = -1;
    if (sleeper2bool) {
        int sleeper2_location = -1;
    }

    //points at the first engine found in our train
    for (int i = 0; i < length; i++) {
        if (sleeper1->isEngine()) {
            sleeper1_location = i;
            break;
        }
        sleeper1=sleeper1->next;
    }

    if (sleeper2bool) {
        TrainCar* sleeper2 = sleeper1;
        //points at the second engine found in our train
        for (int i = sleeper1_location+1; i < length; i++) {
            if (sleeper2->isEngine()) {
                sleeper2_location = i;
                break;
            }
            sleeper2=sleeper2->next;
        }
    }

    //engine to sleeper distance
    bool second = false;
    int distance1 = 0;
    if (num_engines > 1) {
        int distance2 = 0;
        second = true;
    }

    distance1 = std::abs (sleeper1_location - engine1_location);
    if (engine2bool && sleeper2bool) {
        distance2 = std::abs(sleeper2_location - engine2_location);
    }
    else if (sleeper2bool && num_sleeping_cars == 1) {
        distance2 = std::abs(sleeper1_location - engine2_location);
    }

    int closest = distance1;
    if(second) {
        closest = std::min(distance1,distance2);
    }

    return closest;
    */
    int thing = 0;
    return thing; 
}
