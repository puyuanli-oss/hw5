#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here


// Add your implementation of schedule() and other helper functions here
static bool workerInDay(const DailySchedule& sched, size_t day, Worker_T worker);
static bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsUsed,
    size_t day
);

static bool workerInDay(const DailySchedule& sched, size_t day, Worker_T worker)
{
    for (Worker_T w : sched[day])
    {
        if (w == worker)
        {
            return true;
        }
    }
    return false;
}


static bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsUsed,
    size_t day
)
{
    // Finished all days successfully
    if (day == avail.size())
    {
        return true;
    }

    // Current day is complete, move to the next day
    if (sched[day].size() == dailyNeed)
    {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1);
    }

    // Try every worker for this day
    for (size_t worker = 0; worker < avail[day].size(); ++worker)
    {
        if (avail[day][worker] &&
            shiftsUsed[worker] < maxShifts &&
            !workerInDay(sched, day, static_cast<Worker_T>(worker)))
        {
            sched[day].push_back(static_cast<Worker_T>(worker));
            ++shiftsUsed[worker];

            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day))
            {
                return true;
            }

            // Backtrack
            --shiftsUsed[worker];
            sched[day].pop_back();
        }
    }

    return false;
}

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    sched.resize(avail.size());

    // Assume a well-formed matrix: all rows have the same number of workers
    const size_t numWorkers = avail[0].size();

    // If there are no workers but we need some, impossible
    if (dailyNeed > numWorkers)
    {
        return false;
    }

    // Track how many shifts each worker has already been assigned
    vector<size_t> shiftsUsed(numWorkers, 0);

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0);

}

