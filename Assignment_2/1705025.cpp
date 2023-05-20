#include <cstdio>
#include <cmath>
#include "lcgrand.h" /* Header file for random-number generator. */
#include <fstream>
#include <iostream>


using namespace std;


#define INF 1.0e+30



int amount, bigs, initial_inv_level, inv_level, next_event_type, num_events,
num_months, num_values_demand, smalls;


float area_holding, area_shortage, holding_cost, incremental_cost, maxlag,
mean_interdemand, minlag, prob_distrib_demand[26], setup_cost,
shortage_cost, sim_time, time_last_event, time_next_event[5],
total_ordering_cost;


ifstream in;
ofstream out;



void initialize(void);
void timing(void);
void order_arrival(void);
void demand(void);
void evaluate(void);
void report(void);
void update_time_avg_stats(void);
float expon(float mean);
int random_integer(float prob_distrib []);
float uniform(float a, float b);


int main()
{
    int num_policies;

    in.open("input.txt");
    out.open("output.txt");

    num_events = 4;

    //read paraameters
    in >> initial_inv_level >> num_months >> num_policies >> num_values_demand
       >>mean_interdemand >> setup_cost >> incremental_cost >> holding_cost
       >> shortage_cost >> minlag >> maxlag;


    //get probability distribution of demand
    for(int i=1;i<=num_values_demand;i++)
    {
        in >> prob_distrib_demand[i];
    }


    //report writing
    out <<"Single-product inventory system\n\n";
    out << "\nInitial inventory level:\t" << initial_inv_level << endl;
    out<<"\nNumber of demand sizes:\t"<< num_values_demand<<endl;
    out<< "\nDistribution of demand sizes:"<<endl;
    for(int i=1;i<=num_values_demand;i++)
    {
        out<<prob_distrib_demand[i]<<"\t\t";
    }
    out<<endl;
    out<<"\nMean interdemand time:\t"<<mean_interdemand<<" month"<<endl;
    out<<"\nDelivary lag range:\t"<<minlag<<" to "<<maxlag<<" months"<<endl;
    out<<"\nLength of the simulation:\t"<<num_months<<" months"<<endl;
    out<<"\nK: "<<setup_cost<<"\ti: "<<incremental_cost<<"\th: "<<holding_cost<<"\tpi: "<<shortage_cost<<endl;
    out<<"\nNumber of policies: "<<num_policies<<endl;
   
    out<< "\nPolicy\t\tAvg. tot\t\tAvg. ord";
    out<< "\t\tAvg. hold\t\tAvg.short";
    //Run the simulation

    for(int i=1;i<=num_policies;i++)
    {
        initialize();


        do
        {
            //Determine the next event
            timing();
            update_time_avg_stats();

            switch(next_event_type)
            {
                case 1:
                    order_arrival();
                    break;
                case 2:
                    demand();
                    break;
                case 4:
                    evaluate();
                    break;
                case 3:
                    report();
                    break;
            }
        } while (next_event_type != 3);

       
        
    }


    //close the files
    in.close();
    out.close();

    return 0;
}


void initialize()
{
    //initialize the simulation clock

    in>>smalls>>bigs;

    sim_time = 0.0;

    //initialize the state variables
    inv_level = initial_inv_level;
    time_last_event = 0.0;

    //initialize the statistical counters
    total_ordering_cost = 0.0;
    area_holding = 0.0;
    area_shortage = 0.0;

    //initialize the event list
    //order arrival
    time_next_event[1] = INF;
    //demand
    time_next_event[2] = sim_time + expon(mean_interdemand);
    //end of simulation
    time_next_event[3] = num_months;
    //evaluate
    time_next_event[4] = 0.0;

}




void timing(void){  /* Timing function. */

    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */
    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event) {
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */
    if (next_event_type == 0) {

        /* The event list is empty, so stop the simulation. */
        out<<"\nEvent list empty at time " <<sim_time <<" \n";
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */
    sim_time = min_time_next_event;
}


void order_arrival()
{
    inv_level+=amount;
    time_next_event[1] = INF;
}

void demand()
{
    inv_level-= random_integer(prob_distrib_demand);
    time_next_event[2] = sim_time + expon(mean_interdemand);
}

void evaluate()
{
    if(inv_level<smalls)
    {
        amount = bigs - inv_level;
        total_ordering_cost += setup_cost + incremental_cost*amount;
        time_next_event[1] = sim_time + uniform(minlag, maxlag);
    }

    time_next_event[4] = sim_time + 1.0;
}

void report()
{
    float avg_holding_cost, avg_ordering_cost, avg_shortage_cost, total_cost;

    avg_ordering_cost = total_ordering_cost/num_months;
    avg_holding_cost = holding_cost*area_holding/num_months;
    avg_shortage_cost = shortage_cost*area_shortage/num_months;
    total_cost = avg_ordering_cost + avg_holding_cost + avg_shortage_cost;


    out<<"\n\n("<<smalls<<", "<<bigs<<")\t\t"<<total_cost<<"\t\t"<<avg_ordering_cost<<"\t\t"<<avg_holding_cost<<"\t\t"<<avg_shortage_cost;

}


void update_time_avg_stats()
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */
    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */
    area_holding      += max(inv_level,0) * time_since_last_event;

    /* Update area under server-busy indicator function. */
    area_shortage += max(-inv_level, 0) * time_since_last_event;
}


int random_integer(float prob_distrib[])
{
    float u = lcgrand(1);

    int i=1;
    for(; u>=prob_distrib[i];i++);

    return i;
}


float uniform(float a , float b)
{
    return a+(b-a)*lcgrand(1);
}

float expon(float mean){  /* Exponential variate generation function. */
    /* Return an exponential random variate with mean "mean". */
    return -mean * log(lcgrand(1));
}