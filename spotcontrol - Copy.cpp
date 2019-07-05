#include "spotcontrol.h"
#include <QObject>
#include <QDebug>


SpotControl::SpotControl() : QObject()
{
    moving = false;

    topLeftX = 0;
    topLeftY = 0;
    bottomRightX = 0;
    bottomRightY = 0;
    ZDist = 15000;


    //Initialize Reagent List
    for (int i=0;i<8;i++)
        for (int j=0;j<15;j++)
            reagAssign[i][j] = -1;
}

//Start spotting initiated, begin execution loop
void SpotControl::startSpotting()
{
    //Clear the Step List
    stepList.clear();


    //Make a list of all reagents being used in sensor
    bool reagUsed[20];

    for (int i = 0;i<20;i++)
        reagUsed[i] = false;

    for (int i = 0;i<8;i++)
        for (int j = 0;j<15;j++)
            if (reagAssign[i][j] != -1)
                reagUsed[reagAssign[i][j]] = true;


    //For each reagent that is being used, add relevent execution steps
    for (int i =0;i<20;i++)
        if(reagUsed[i])
            addReagentSensorCoords(i);


    totNumSteps = stepList.size();

    emit progress((1-stepList.size()/totNumSteps)*100);

    //Start Execution
    processStep();

}

//For a given reagent (known to exist in current configuration), add all required steps to
//  execution loop to grab pipette tip, dose every location relevant to the reagent, and eject the tip
void SpotControl::addReagentSensorCoords(int reag)
{
    //Pick up pipette tip~~~~~~~~~~~~~~~~
    step pipetteGrab;

    //Position over pipette tip
    pipetteGrab.move = true;
    pipetteGrab.x_pos = PIPETTE_COORDS[reag][0];
    pipetteGrab.y_pos = PIPETTE_COORDS[reag][1];
    pipetteGrab.z_pos = 15000;
    pipetteGrab.pump = false;
    pipetteGrab.valve = false;
    stepList.append(pipetteGrab);

    //Pick up tip
    pipetteGrab.z_pos = 77000;
    stepList.append(pipetteGrab);
    //Pick up tip
    pipetteGrab.z_pos = 77000;
    stepList.append(pipetteGrab);

    //Raise again
    pipetteGrab.z_pos = 15000;
    stepList.append(pipetteGrab);


    //For each location used, grab some reagent and spot
    for (int i = 0;i<8;i++)
    {
        for (int j = 0;j<15;j++)
        {
            if (reagAssign[i][j] ==reag)
            {
                //Grab reagent~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                step reagGrab;

                //Move over reagent well
                reagGrab.move = true;
                reagGrab.x_pos = REAG_COORDS[reag][0];
                reagGrab.y_pos = REAG_COORDS[reag][1];
                reagGrab.z_pos = 60000;
                reagGrab.pump = false;
                reagGrab.valve = false;
                stepList.append(reagGrab);

                //Dip into reagent well
                reagGrab.move = true;
                reagGrab.x_pos = REAG_COORDS[reag][0];
                reagGrab.y_pos = REAG_COORDS[reag][1];
                reagGrab.z_pos = 83000;
                stepList.append(reagGrab);

                //Set valve to input
                reagGrab.valve = true;
                reagGrab.valveIn = true;
                reagGrab.move = false;
                reagGrab.pump = false;
                stepList.append(reagGrab);

                //Draw liquid
                reagGrab.move = false;
                reagGrab.pump = true;
                reagGrab.valve = false;
                reagGrab.valveIn = false;
                reagGrab.pumpPos = 25;
                stepList.append(reagGrab);

                //Rise back above well height
                reagGrab.move = true;
                reagGrab.pump = false;
                reagGrab.valve = false;
                reagGrab.x_pos = REAG_COORDS[reag][0];
                reagGrab.y_pos = REAG_COORDS[reag][1];
                reagGrab.z_pos = 60000;
                stepList.append(reagGrab);

                //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                step spotDispense;

                //Calculate x-y coordinates based upon corner coordinates

                int xrange = bottomRightX-topLeftX;
                int yrange = topLeftY-bottomRightY;

                int x_pos = topLeftX+i*xrange/8;
                int y_pos = topLeftY-j*yrange/15;

                //Move to sensor position
                spotDispense.move = true;
                spotDispense.valve = false;
                spotDispense.pump = false;
                spotDispense.x_pos = x_pos;
                spotDispense.y_pos = y_pos;
                spotDispense.z_pos = 60000;
                stepList.append(spotDispense);

                //Lower to sensor
                spotDispense.z_pos = ZDist;
                stepList.append(spotDispense);

                //Set valve to output
                spotDispense.move = false;
                spotDispense.valve = true;
                spotDispense.pump = false;
                spotDispense.valveIn = false;
                stepList.append(spotDispense);

                //Draw more air
                spotDispense.valve = false;
                spotDispense.move = false;
                spotDispense.valveIn = false;
                spotDispense.pump = true;
                spotDispense.pumpPos = 400;
                stepList.append(spotDispense);

                //Pause
                spotDispense.move = true;
                spotDispense.valve = false;
                spotDispense.pump = false;
                stepList.append(spotDispense);

                //Set valve to input
                spotDispense.pump = false;
                spotDispense.move = false;
                spotDispense.valve = true;
                spotDispense.valveIn = true;
                stepList.append(spotDispense);

                //Dispense liquid
                spotDispense.move = false;
                spotDispense.pump = true;
                spotDispense.valve = false;
                spotDispense.pumpPos = 0;
                stepList.append(spotDispense);

                //Raise
                spotDispense.move = true;
                spotDispense.pump = false;
                spotDispense.valve = false;
                spotDispense.z_pos = 60000;
                stepList.append(spotDispense);

            }

        }
    }

    ejectPipette();
}

//Receive the reagent assignments from the GUI
void SpotControl::rxReagAssignments(int reag[8][15])
{
    //Initialize Reagent List
    for (int i=0;i<8;i++)
        for (int j=0;j<15;j++)
            reagAssign[i][j] = -1;

    //Copy reagent assignments to local vars
    for (int i = 0;i<8;i++)
        for (int j = 0;j<15;j++)
            reagAssign[i][j] = reag[i][j];


}

//Receives sensor coordinates from GUI
void SpotControl::rxStartCoordinates(int x1,int y1,int x2,int y2,int z)
{
    topLeftX = x1;
    topLeftY = y1;
    bottomRightX = x2;
    bottomRightY = y2;
    ZDist = z;
}

//Executes a single step in the list
void SpotControl::processStep()
{
    //If list still has steps
    if (stepList.size() > 0)
    {
        //Pull off top of list
        step curStep = stepList.at(0);
        stepList.removeFirst();

        //Update progress
        emit progress((1-stepList.size()/totNumSteps)*100);

        //Step is a robot step
        if (curStep.move == true)
        {
            //Move to specified location
            moving = true;
            emit moveToPos(curStep.x_pos,curStep.y_pos,curStep.z_pos);
        }
        //Step is a pump step
        else if (curStep.pump == true)
        {
            moving = true;
            emit setPlungerPos(curStep.pumpPos);
        }
        else if (curStep.valve == true)
        {
            moving = true;
            emit setValveDir(curStep.valveIn);
        }

    }
    else
    {
        //Process Complete
        emit progress(100);
        emit complete();
    }

}

//Robot movement is complete
void SpotControl::doneMoving()
{
    moving = false;
    processStep();

}

//Used to insert a pipette tip ejection into the command queue
void SpotControl::ejectPipette()
{
    //First step, move to waste bin
    step eject;
    eject.move = true;
    eject.x_pos = 20000;
    eject.y_pos = 25000;
    eject.z_pos = 15000;
    eject.pump = false;
    eject.valve = false;
    stepList.append(eject);

    //Second step, retract z
    eject.move = true;
    eject.z_pos = 5000;
    stepList.append(eject);

    //Third step, lower z axis again
    eject.move = true;
    eject.z_pos = 15000;
    stepList.append(eject);


}

