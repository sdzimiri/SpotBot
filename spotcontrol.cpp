#include "spotcontrol.h"
#include <QObject>
#include <windows.h>
#include <QDebug>


SpotControl::SpotControl() : QObject()
{
    sensor_version = 2;

    moving = false;
    sleeping = false;

    topLeftX = 0;
    topLeftY = 0;
    bottomRightX = 0;
    bottomRightY = 0;
    ZDist = 15000;
    stepNum = 0;

    //Initialize Reagent List
    for (int i=0;i<8;i++)
        for (int j=0;j<15;j++)
            reagAssign[i][j] = -1;

    for (int i=0;i<8;i++)
        reagAssign_v3[i] = -1;

    for (int i=0; i<2; i++)
        for (int j=0; j<8; j++)
            reagAssign_v4[i][j] = -1;

    //Initialize test spots
    testSpotOne = -1;
    testSpotTwo = -1;

    //sleepThread = new QThread();
    sTimer = new QTimer(this);
    sTimer->setSingleShot(true);
    //sTimer->moveToThread(sleepThread);
    connect(sTimer,SIGNAL(timeout()),this,SLOT(rxSleepTimerDone()));

}

//Start spotting initiated, begin execution loop
void SpotControl::startSpotting()
{
    //Clear the Step List
    stepList.clear();
    stepNum = 0;

    //Make a list of all reagents being used in sensor
    bool reagUsed[20];

    for (int i = 0;i<20;i++)
        reagUsed[i] = false;

    if ((sensor_version ==1) || (sensor_version == 2))
    {
        for (int i = 0;i<8;i++)
            for (int j = 0;j<15;j++)
                if (reagAssign[i][j] != -1)
                    reagUsed[reagAssign[i][j]] = true;
    }
    else if (sensor_version == 3)
    {
        for (int i = 0;i<8;i++)
            if (reagAssign_v3[i] != -1)
                reagUsed[reagAssign_v3[i]] = true;
    }
    else if (sensor_version == 4)
    {
        for (int i = 0;i<2;i++)
            for (int j = 0;j<8;j++)
                if (reagAssign_v4[i][j] != -1)
                    reagUsed[reagAssign_v4[i][j]] = true;
    }
    else if (sensor_version == 5)
    {
        for (int i = 0;i<6;i++)
            for (int j = 0;j<4;j++)
                if (reagAssign_hs[i][j] != -1)
                    reagUsed[reagAssign_hs[i][j]] = true;
    }

    //For each test spot that is being used, add relevant execution steps
    if(testSpotOne > -1)
        addTestSpot(testSpotOne,1);

    if(testSpotTwo > -1)
        addTestSpot(testSpotTwo,2);

    //For each reagent that is being used, add relevant execution steps
    //NEW
    //Loop for total script iterations, adding the reagent sensor coordinates for each instance through the loop
    for (int j=0;j<scriptIterations;j++)
        for (int i =0;i<20;i++)
            if(reagUsed[i])
            {
                if ((sensor_version == 1) || (sensor_version == 2))
                    addReagentSensorCoordsSpot_v12(i);
                else if (sensor_version == 3)
                    addReagentSensorCoordsSpot_v3(i);
                else if (sensor_version == 4)
                    addReagentSensorCoordsSpot_v4(i);
                else if (sensor_version == 5)
                    addReagentSensorCoordsSpot_hs(i);
            }

    totNumSteps = stepList.size();

    emit progress((1-stepList.size()/totNumSteps)*100);

    //Start Execution
    processStep();

}

//For a given test spot (num = 1 or 2), add the reagent to the test spot coordinates
void SpotControl::addTestSpot(int reag, int num)
{
    //Perform rinse
    step reagGrab;

    //Move over rinse well one
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*3;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well two
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*2;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well three
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Wait 10 seconds for EtOH to evaporate
    reagGrab.sleepTime = 10000;
    stepList.append(reagGrab);

    //Set coordinates of corner spots
    int x_pos1;
    int y_pos1;

    int x_pos2;
    int y_pos2;

    if (num == 1)
    {
        qDebug() << "Adding Test Spot One";

        if (sensor_version == 1)
        {
            //Top Left
            x_pos1 = r18c13_cent_x-v1_sensor_pitch_x*steps_per_micron*2;
            y_pos1 = r18c13_cent_y+v1_sensor_pitch_y*steps_per_micron*2;

            //Bottom Right
            x_pos2 = r18c13_cent_x+v1_sensor_pitch_x*steps_per_micron*16;
            y_pos2 = r18c13_cent_y-v1_sensor_pitch_y*steps_per_micron*9;
        }
        else if (sensor_version == 2)
        {
            //Top Left
            x_pos1 = r18c13_cent_x-v2_sensor_pitch_x*steps_per_micron;
            y_pos1 = r18c13_cent_y+v2_sensor_pitch_y*steps_per_micron*2;

            //Bottom Right
            x_pos2 = r18c13_cent_x+v2_sensor_pitch_x*steps_per_micron*9;
            y_pos2 = r18c13_cent_y-v2_sensor_pitch_y*steps_per_micron*9;
        }
    }

    if (num == 2)
    {
        qDebug() << "Adding Test Spot Two";

        if (sensor_version == 1)
        {
            //Top Right
            x_pos1 = r18c13_cent_x-v1_sensor_pitch_x*steps_per_micron*2;
            y_pos1 = r18c13_cent_y-v1_sensor_pitch_y*steps_per_micron*9;

            //Bottom Left
            x_pos2 = r18c13_cent_x+v1_sensor_pitch_x*steps_per_micron*16;
            y_pos2 = r18c13_cent_y+v1_sensor_pitch_y*steps_per_micron*2;
        }
        else if (sensor_version == 2)
        {
            //Top Right
            x_pos1 = r18c13_cent_x-v2_sensor_pitch_x*steps_per_micron;
            y_pos1 = r18c13_cent_y-v2_sensor_pitch_y*steps_per_micron*9;

            //Bottom Left
            x_pos2 = r18c13_cent_x+v2_sensor_pitch_x*steps_per_micron*9;
            y_pos2 = r18c13_cent_y+v2_sensor_pitch_y*steps_per_micron*2;
        }
    }

    //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    step spotDispense;

    // FIRST SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Pickup reagent

    spotDispense.sleepTime = 0;
    spotDispense.x_pos = REAG_COORDS[reag][0];
    spotDispense.y_pos = REAG_COORDS[reag][1];
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    spotDispense.sleepTime = 0;
    spotDispense.z_pos = REAG_PICKUP_Z;
    stepList.append(spotDispense);

    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    //Move to sensor position (0,0)
    spotDispense.sleepTime = 0;
    spotDispense.x_pos = x_pos1;
    spotDispense.y_pos = y_pos1;
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    //Lower to sensor
    spotDispense.sleepTime = 0;
    spotDispense.z_pos = ZDist;
    stepList.append(spotDispense);

    //Raise
    spotDispense.sleepTime = 0;
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    // SECOND SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Pickup reagent

    spotDispense.sleepTime = 0;
    spotDispense.x_pos = REAG_COORDS[reag][0];
    spotDispense.y_pos = REAG_COORDS[reag][1];
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    spotDispense.sleepTime = 0;
    spotDispense.z_pos = REAG_PICKUP_Z;
    stepList.append(spotDispense);

    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    //Move to sensor position (0,0)
    spotDispense.sleepTime = 0;
    spotDispense.x_pos = x_pos2;
    spotDispense.y_pos = y_pos2;
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);

    //Lower to sensor
    spotDispense.sleepTime = 0;
    spotDispense.z_pos = ZDist;
    stepList.append(spotDispense);

    //Raise
    spotDispense.sleepTime = 0;
    spotDispense.z_pos = 85000;
    stepList.append(spotDispense);


}


//SENSOR VERSION 1 AND VERSION 2
//For a given reagent (known to exist in current configuration), add all required steps to
//  execution loop to grab pipette tip, dose every location relevant to the reagent, and eject the tip
// THIS FUNCTION FILLS THE PIPETTE AND THEN TAPS THE TIP ON THE SENSOR TO SPOT
void SpotControl::addReagentSensorCoordsSpot_v12(int reag)
{
    //Calculate how many times this reagent will be used
    int count= 0;

    for (int i = 0;i<8;i++)
        for (int j = 0;j<15;j++)
            if (reagAssign[i][j] ==reag)
                count = count +1;


    //Perform rinse
    step reagGrab;

    //Move over rinse well one
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*5;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well two
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*4;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well three
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*3;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well four
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*2;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well five
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Wait 10 seconds for EtOH to evaporate
    reagGrab.sleepTime = 10000;
    stepList.append(reagGrab);


    //For each location used, Spot
    for (int i = 0;i<8;i++)
    {
        for (int j = 0;j<15;j++)
        {
            if (reagAssign[i][j] ==reag)
            {
                //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                step spotDispense;
\
                int x_pos;
                int y_pos;

                //Calculate x-y coordinates based upon corner coordinates
                if (sensor_version == 1)
                {
                    x_pos = r18c13_cent_x+v1_sensor_pitch_x*steps_per_micron*j;
                    y_pos = r18c13_cent_y-v1_sensor_pitch_y*steps_per_micron*i;
                }
                else if (sensor_version == 2)
                {
                    //new sensor row order is different
                    //This code simply translates the coordinates
                    int new_row;

                    switch(j)
                    {
                        case 0:
                            new_row = 0;
                            break;
                        case 1:
                            new_row = 1;
                            break;
                        case 3:
                            new_row = 2;
                            break;
                        case 5:
                            new_row = 3;
                            break;
                        case 7:
                            new_row = 4;
                            break;
                        case 9:
                            new_row = 5;
                            break;
                        case 12:
                            new_row = 6;
                            break;
                        case 13:
                            new_row = 7;
                            break;
                        case 14:
                            new_row = 8;
                            break;
                        case 4:
                            new_row = 2;
                            break;
                        case 6:
                            new_row = 3;
                            break;
                        case 8:
                            new_row = 4;
                            break;
                        case 10:
                            new_row = 5;
                            break;

                    }

                    if (i == 7) //column 20 is now on row 0
                    {
                        new_row = 0;
                    }

                    //If odd column, subtract offset (staggered array)
                    if (i%2)
                        x_pos = r18c13_cent_x+v2_sensor_pitch_x*steps_per_micron*new_row-v2_sensor_column_stagger;
                    else
                        x_pos = r18c13_cent_x+v2_sensor_pitch_x*steps_per_micron*new_row;

                    y_pos = r18c13_cent_y-v2_sensor_pitch_y*steps_per_micron*i;

                }

                // SPOT SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //Pickup reagent
                //spotDispense.sleepTime = SPOT_DRY_TIME;
                //stepList.append(spotDispense);

                spotDispense.sleepTime = 0;
                spotDispense.x_pos = REAG_COORDS[reag][0];
                spotDispense.y_pos = REAG_COORDS[reag][1];
                spotDispense.z_pos = 85000;
                stepList.append(spotDispense);

                spotDispense.sleepTime = 0;
                spotDispense.z_pos = REAG_PICKUP_Z;
                stepList.append(spotDispense);

                spotDispense.z_pos = 85000;
                stepList.append(spotDispense);

                //Move to sensor position (0,0)
                spotDispense.sleepTime = 0;
                spotDispense.x_pos = x_pos;
                spotDispense.y_pos = y_pos;
                spotDispense.z_pos = 85000;
                stepList.append(spotDispense);

                //Lower to sensor
                spotDispense.sleepTime = 0;
                spotDispense.z_pos = ZDist;
                stepList.append(spotDispense);

                //Raise
                spotDispense.sleepTime = 0;
                spotDispense.z_pos = 85000;
                stepList.append(spotDispense);

            }

        }
    }
}

//SENSOR VERSION 3
//For a given reagent (known to exist in current configuration), add all required steps to
//  execution loop to grab pipette tip, dose every location relevant to the reagent, and eject the tip
// THIS FUNCTION FILLS THE PIPETTE AND THEN TAPS THE TIP ON THE SENSOR TO SPOT
void SpotControl::addReagentSensorCoordsSpot_v3(int reag)
{
    //Calculate how many times this reagent will be used
    int count= 0;

    for (int i = 0;i<8;i++)
        if (reagAssign_v3[i] ==reag)
            count = count +1;


    //Perform rinse
    step reagGrab;

    //Move over rinse well one
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*5;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well two
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*4;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well three
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*3;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well four
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*2;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well five
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Wait 10 seconds for EtOH to evaporate
    reagGrab.sleepTime = 10000;
    stepList.append(reagGrab);


    //For each location used, Spot
    for (int i = 0;i<8;i++)
    {
        if (reagAssign_v3[i] == reag)
        {
            //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            step spotDispense;

            //Calculate x-y center coordinates based upon corner coordinates

            int center_x = v3_sensor1_cen_x + v3_sensor_pitch_x * steps_per_micron*i;
            int center_y = v3_sensor1_cen_y;

            //V3 sensors are large and have a grid of spots defined over them. For each
            //spot in the grid, pick up the reagent and spot

            int x_pitch = float(v3_sensor_size_x+v3_sensor_size_x*float(spot_v3_os)/100*2)/float(spot_v3_vert+1);
            int y_pitch = float(v3_sensor_size_y+v3_sensor_size_y*float(spot_v3_os)/100*2)/float(spot_v3_hor+1);


            int x_pos;
            int y_pos;

            int x_0 = center_x - v3_sensor_size_x*steps_per_micron/2;
            int y_0 = center_y + v3_sensor_size_y*steps_per_micron/2;

            for(int ii=1;ii<=spot_v3_vert;ii++)
            {
                for(int jj=1;jj<=spot_v3_hor;jj++)
                {
                    x_pos = x_0-v3_sensor_size_x*float(spot_v3_os)/100+x_pitch*ii;
                    y_pos = y_0+v3_sensor_size_y*float(spot_v3_os)/100-y_pitch*jj;

                    // SPOT SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    //Pickup reagent
                    //spotDispense.sleepTime = SPOT_DRY_TIME;
                    //stepList.append(spotDispense);

                    spotDispense.sleepTime = 0;
                    spotDispense.x_pos = REAG_COORDS[reag][0];
                    spotDispense.y_pos = REAG_COORDS[reag][1];
                    spotDispense.z_pos = 85000;
                    stepList.append(spotDispense);

                    spotDispense.sleepTime = 0;
                    spotDispense.z_pos = REAG_PICKUP_Z;
                    stepList.append(spotDispense);

                    spotDispense.z_pos = 85000;
                    stepList.append(spotDispense);

                    //Move to sensor position (0,0)
                    spotDispense.sleepTime = 0;
                    spotDispense.x_pos = x_pos;
                    spotDispense.y_pos = y_pos;
                    spotDispense.z_pos = 85000;
                    stepList.append(spotDispense);

                    //Lower to sensor
                    spotDispense.sleepTime = 0;
                    spotDispense.z_pos = ZDist;
                    stepList.append(spotDispense);

                    //Raise
                    spotDispense.sleepTime = 0;
                    spotDispense.z_pos = 85000;
                    stepList.append(spotDispense);
                }
            }


        }
    }
}

//SENSOR VERSION 4
//For a given reagent (known to exist in current configuration), add all required steps to
//  execution loop to grab pipette tip, dose every location relevant to the reagent, and eject the tip
// THIS FUNCTION FILLS THE PIPETTE AND THEN TAPS THE TIP ON THE SENSOR TO SPOT
void SpotControl::addReagentSensorCoordsSpot_v4(int reag)
{
    //Calculate how many times this reagent will be used
    int count= 0;

    for (int i = 0;i<2;i++)
        for (int j = 0; j<8;j++)
            if (reagAssign_v4[i][j] == reag)
                count = count +1;


    //Perform rinse
    step reagGrab;

    //Move over rinse well one
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*5;
    reagGrab.z_pos = 70000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well two
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*4;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well three
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*3;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well four
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*2;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Move over rinse well five
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH;
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = 85000;
    stepList.append(reagGrab);

    //Wait 10 seconds for EtOH to evaporate
    reagGrab.sleepTime = 10000;
    stepList.append(reagGrab);


    //For each location used, Spot
    for (int i = 0;i<2;i++)
    {
        for (int j=0;j<8;j++)
            if (reagAssign_v4[i][j] == reag)
            {
                //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                step spotDispense;

                //Calculate x-y center coordinates based upon corner coordinates

                int center_x = v4_r1c1_cen_x + v4_sensor_pitch_x * steps_per_micron*j;
                int center_y = v4_r1c1_cen_y - v4_sensor_pitch_y * steps_per_micron*i;;

                //V4 sensors are large and have a grid of spots defined over them. For each
                //spot in the grid, pick up the reagent and spot

                int x_pitch = float(v4_sensor_size_x+v4_sensor_size_x*float(spot_v4_os)/100*2)/float(spot_v4_vert+1);
                int y_pitch = float(v4_sensor_size_y+v4_sensor_size_y*float(spot_v4_os)/100*2)/float(spot_v4_hor+1);


                int x_pos;
                int y_pos;

                int x_0 = center_x - v4_sensor_size_x*steps_per_micron/2;
                int y_0 = center_y + v4_sensor_size_y*steps_per_micron/2;

                for(int ii=1;ii<=spot_v4_vert;ii++)
                {
                    for(int jj=1;jj<=spot_v4_hor;jj++)
                    {
                        x_pos = x_0-v4_sensor_size_x*float(spot_v4_os)/100+x_pitch*ii;
                        y_pos = y_0+v4_sensor_size_y*float(spot_v4_os)/100-y_pitch*jj;

                        // SPOT SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        //Pickup reagent
                        //spotDispense.sleepTime = SPOT_DRY_TIME;
                        //stepList.append(spotDispense);

                        spotDispense.sleepTime = 0;
                        spotDispense.x_pos = REAG_COORDS[reag][0];
                        spotDispense.y_pos = REAG_COORDS[reag][1];
                        spotDispense.z_pos = 85000;
                        stepList.append(spotDispense);

                        spotDispense.sleepTime = 0;
                        spotDispense.z_pos = REAG_PICKUP_Z;
                        stepList.append(spotDispense);

                        spotDispense.z_pos = 85000;
                        stepList.append(spotDispense);

                        //Move to sensor position (0,0)
                        spotDispense.sleepTime = 0;
                        spotDispense.x_pos = x_pos;
                        spotDispense.y_pos = y_pos;
                        spotDispense.z_pos = 85000;
                        stepList.append(spotDispense);

                        //Lower to sensor
                        spotDispense.sleepTime = 0;
                        spotDispense.z_pos = ZDist;
                        stepList.append(spotDispense);

                        //Raise
                        spotDispense.sleepTime = 0;
                        spotDispense.z_pos = 85000;
                        stepList.append(spotDispense);
                    }
                }


            }
    }
}

void SpotControl::addReagentSensorCoordsSpot_hs(int reag)
{

    //Calculate how many times this reagent will be used
    int count= 0;

    for (int i = 0;i<6;i++)
        for (int j = 0; j<4;j++)
            if (reagAssign_hs[i][j] == reag)
                count = count +1;


    //Perform rinse
    step reagGrab;

    //Move over rinse well one
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*5;
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Move over rinse well two
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*4;
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Move over rinse well three
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*3;
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Move over rinse well four
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH*2;
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Move over rinse well five
    reagGrab.sleepTime = 0;
    reagGrab.x_pos = REAG_COORDS[reag][0];
    reagGrab.y_pos = REAG_COORDS[reag][1] - WELL_PITCH;
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Dip into rinse buffer and agitate
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z-2000;
    stepList.append(reagGrab);
    reagGrab.z_pos = REAG_PICKUP_Z;
    stepList.append(reagGrab);
    reagGrab.z_pos = JOG_Z;
    stepList.append(reagGrab);

    //Wait 10 seconds for EtOH to evaporate
    reagGrab.sleepTime = 10000;
    stepList.append(reagGrab);


    //For each location used, Spot
    for (int i = 0;i<6;i++)
    {
        for (int j=0;j<4;j++)
            if (reagAssign_hs[i][j] == reag)
            {
                //Spot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                step spotDispense;

                //Calculate x-y center coordinates based upon corner coordinates

                int x_pos = hs_r1c1_cen_x + hs_pitch_x * steps_per_micron*(j);
                int y_pos = hs_r1c1_cen_y - hs_pitch_y * steps_per_micron*(i);;


                // SPOT SPOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //Pickup reagent
                //spotDispense.sleepTime = SPOT_DRY_TIME;
                //stepList.append(spotDispense);

                spotDispense.sleepTime = 0;
                spotDispense.x_pos = REAG_COORDS[reag][0];
                spotDispense.y_pos = REAG_COORDS[reag][1];
                spotDispense.z_pos = JOG_Z;
                stepList.append(spotDispense);

                spotDispense.sleepTime = 0;
                spotDispense.z_pos = REAG_PICKUP_Z;
                stepList.append(spotDispense);

                spotDispense.z_pos = JOG_Z;
                stepList.append(spotDispense);

                //Move to sensor position (0,0)
                spotDispense.sleepTime = 0;
                spotDispense.x_pos = x_pos;
                spotDispense.y_pos = y_pos;
                spotDispense.z_pos = JOG_Z;
                stepList.append(spotDispense);

                //Lower to sensor
                spotDispense.sleepTime = 0;
                spotDispense.z_pos = ZDist;
                stepList.append(spotDispense);

                //Raise
                spotDispense.sleepTime = 0;
                spotDispense.z_pos = JOG_Z;
                stepList.append(spotDispense);

        }
    }
}

//Receive the reagent assignments from the GUI
void SpotControl::rxReagAssignments(int reag[8][15], int reag_v3[8], int reag_v4[2][8], int reag_hs[6][4])
{
    //Initialize Reagent List
    for (int i=0;i<8;i++)
        for (int j=0;j<15;j++)
            reagAssign[i][j] = -1;
    for (int i=0;i<8;i++)
        reagAssign_v3[i] = -1;
    for (int i=0; i<2; i++)
        for (int j=0; j<8; j++)
            reagAssign_v4[i][j] = -1;
    for (int i=0; i<6; i++)
        for (int j=0; j<4; j++)
            reagAssign_hs[i][j] = -1;

    //Copy reagent assignments to local vars
    for (int i = 0;i<8;i++)
        for (int j = 0;j<15;j++)
            reagAssign[i][j] = reag[i][j];
    for (int i=0;i<8;i++)
        reagAssign_v3[i] = reag_v3[i];
    for (int i=0; i<2; i++)
        for (int j=0; j<8; j++)
            reagAssign_v4[i][j] = reag_v4[i][j];
    for (int i=0; i<6; i++)
        for (int j=0; j<4; j++)
            reagAssign_hs[i][j] = reag_hs[i][j];


}

//Receive the spotting configuration (v3) from the GUI
void SpotControl::rxSpotConfigV3(int hor, int vert, int overshoot)
{
    spot_v3_hor = hor;
    spot_v3_vert = vert;
    spot_v3_os = overshoot;
}

//Receive the spotting configuration (v4) from the GUI
void SpotControl::rxSpotConfigV4(int hor, int vert, int overshoot)
{
    spot_v4_hor = hor;
    spot_v4_vert = vert;
    spot_v4_os = overshoot;
}

void SpotControl::rxTestSpots(int spot1, int spot2)
{
    testSpotOne = spot1;
    testSpotTwo = spot2;
}

//Receives datum coordinates from GUI
//Performs initial calibration calculations
void SpotControl::rxStartCoordinates(int x1,int y1,int x2,int y2,int z)
{
    topLeftX = x1;
    topLeftY = y1;
    bottomRightX = x2;
    bottomRightY = y2;
    ZDist = z;

    //Calculate microns per step
    //float h = (float)sqrt((long double)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
    steps_per_micron = 1; //h/datum_delta;

    //Calculate center coordinates of top left sensor (r18c13)
    //All other coordinates are calculated from this based upon the pitch of the sensors
    if (sensor_version == 1)
    {
        r18c13_cent_y = y1 - (v1_r18c13_cent_y_um)*steps_per_micron - (v1_sensor_size_y/2)*steps_per_micron;   //step coordinates of top left sensor
        r18c13_cent_x = x1 + (v1_r18c13_cent_x_um)*steps_per_micron + (v1_sensor_size_x/2)*steps_per_micron;   //step coordinates of top left sensor
    }
    else if (sensor_version == 2)
    {
        r18c13_cent_y = y1 - (v2_r18c13_cent_y_um)*steps_per_micron - (v2_sensor_size_y/2)*steps_per_micron;   //step coordinates of top left sensor
        r18c13_cent_x = x1 + (v2_r18c13_cent_x_um)*steps_per_micron + (v2_sensor_size_x/2)*steps_per_micron;   //step coordinates of top left sensor
    }
    else if (sensor_version == 3)
    {
        v3_sensor1_cen_y = y1 - (v3_1_cen_y_um)*steps_per_micron;   //step coordinates of top left sensor
        v3_sensor1_cen_x = x1 + (v3_1_cen_x_um)*steps_per_micron;   //step coordinates of top left sensor
    }
    else if (sensor_version == 4)
    {
        v4_r1c1_cen_y = y1 - (v4_1_cen_y_um)*steps_per_micron;   //step coordinates of top left sensor
        v4_r1c1_cen_x = x1 + (v4_1_cen_x_um)*steps_per_micron;   //step coordinates of top left sensor
    }
    else if (sensor_version == 5)
    {
        qDebug() << "RECEIVED HS COORDS";
        hs_r1c1_cen_y = y1*steps_per_micron;   //step coordinates of top left aperture
        hs_r1c1_cen_x = x1*steps_per_micron;   //step coordinates of top left aperture
    }

    //Calculate rotation
    //Based off top left coordinate, assuming zero rotation is sa horizontal line
    //if (x2 > x1)
    //    theta = atan((long double)((x2-x1)/(y2-y1)));
   // else
     //   theta = -1*atan((long double)((x2-x1)/(y2-y1)));

    //This line disable rotation correction
    theta = 0;

}

void SpotControl::rxScriptLoopCnt(int loopCnt)
{

    scriptIterations = loopCnt;
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
        emit progress(((1-(float)stepList.size()/(float)totNumSteps))*100);

        if (curStep.sleepTime > 0)
        {
            sleeping = true;
            sTimer->start(curStep.sleepTime);
        }
        else
        {
            //Move to specified location
            moving = true;
            emit moveToPos(curStep.x_pos,curStep.y_pos,curStep.z_pos);
        }

        qDebug() << "Executing Step " + QString::number(stepNum) + ", " + QString::number(stepList.size()) + " Remain.";
        stepNum++;


    }
    else
    {
        //Process Complete
        emit complete();
    }

}

//Robot movement is complete
void SpotControl::doneMoving()
{
    if (moving == true)
    {
        moving = false;
        processStep();
    }

}

void SpotControl::rxSleepTimerDone()
{
    if (sleeping == true)
    {
        sleeping = false;
        processStep();
    }
}

void SpotControl::rxSensorVersion(int version)
{
    sensor_version = version;
}

//Error received from robot. If running automated protocol, stop.
void SpotControl::rxRobotError()
{
    //Clear list
    stepList.clear();
    moving = false;

    //Send complete
    emit complete();
}

//Used for manually moving to a coordinate
void SpotControl::moveToCoord(int datum1_x1, int datum1_y1, int col, int row)
{

    int r18c13_cent_y_;
    int r18c13_cent_x_;
    int steps_per_micron_ = 1;

    if (sensor_version == 1)
    {
        r18c13_cent_y_ = datum1_y1 - (v1_r18c13_cent_y_um)*steps_per_micron_ - (v1_sensor_size_y/2)*steps_per_micron_;   //step coordinates of top left sensor
        r18c13_cent_x_ = datum1_x1 + (v1_r18c13_cent_x_um)*steps_per_micron_ + (v1_sensor_size_x/2)*steps_per_micron_;   //step coordinates of top left sensor
    }
    else if (sensor_version == 2)
    {
        r18c13_cent_y_ = datum1_y1 - (v2_r18c13_cent_y_um)*steps_per_micron_ - (v2_sensor_size_y/2)*steps_per_micron_;   //step coordinates of top left sensor
        r18c13_cent_x_ = datum1_x1 + (v2_r18c13_cent_x_um)*steps_per_micron_ + (v2_sensor_size_x/2)*steps_per_micron_;   //step coordinates of top left sensor
    }

    int x_pos = 0;
    int y_pos = 0;

    //Calculate x-y coordinates based upon corner coordinates
    if (sensor_version == 1)
    {
        x_pos = r18c13_cent_x_+v1_sensor_pitch_x*steps_per_micron_*row;
        y_pos = r18c13_cent_y_-v1_sensor_pitch_y*steps_per_micron_*col;
    }
    else if (sensor_version == 2)
    {
        //new sensor row order is different
        //This code simply translates the coordinates
        int new_row;

        switch(row)
        {
            case 0:
                new_row = 0;
                break;
            case 1:
                new_row = 1;
                break;
            case 3:
                new_row = 2;
                break;
            case 5:
                new_row = 3;
                break;
            case 7:
                new_row = 4;
                break;
            case 9:
                new_row = 5;
                break;
            case 12:
                new_row = 6;
                break;
            case 13:
                new_row = 7;
                break;
            case 14:
                new_row = 8;
                break;
            case 4:
                new_row = 2;
                break;
            case 6:
                new_row = 3;
                break;
            case 8:
                new_row = 4;
                break;
            case 10:
                new_row = 5;
                break;

        }

        if (row == 7) //column 20 is now on row 0
        {
            new_row = 0;
        }

        //If odd column, subtract offset (staggered array)
        if (col%2)
            x_pos = r18c13_cent_x_+v2_sensor_pitch_x*steps_per_micron_*new_row-v2_sensor_column_stagger;
        else
            x_pos = r18c13_cent_x_+v2_sensor_pitch_x*steps_per_micron_*new_row;

        y_pos = r18c13_cent_y_-v2_sensor_pitch_y*steps_per_micron_*col;

    }



    emit moveToPos(x_pos,y_pos,0);

}


