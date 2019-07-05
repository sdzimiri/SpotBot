//Provides an automation class to execute automated spotting, waiting, etc to prevent the GUI thread from locking up.

#ifndef SPOTCONTROL_H
#define SPOTCONTROL_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QThread>

//Sensor Geometry, version 1
#define v1_r18c13_cent_y_um 6760  //relative distance (x) from top left datum to center of top left sensor, in um
#define v1_r18c13_cent_x_um 3669.5  //relative distance (y) from top left datum to center of top left sensor, in um
#define v1_sensor_pitch_y 510 //um
#define v1_sensor_pitch_x 120 //um
#define v1_sensor_size_x 112.5 //um
#define v1_sensor_size_y 220 //um
#define v1_datum_delta 16760 //um, distance between the datum points

//Sensor Geometry, version 2
#define v2_r18c13_cent_y_um 8991.676  //relative distance (x) from top left datum to center of top left sensor, in um (sensor version 2)
#define v2_r18c13_cent_x_um 4878.126  //relative distance (y) from top left datum to center of top left sensor, in um (sensor version 2)
#define v2_sensor_pitch_y 510 //um
#define v2_sensor_pitch_x 870 //um
#define v2_sensor_column_stagger 435 //um
#define v2_sensor_size_x 112.5 //um
#define v2_sensor_size_y 220 //um
#define v2_datum_delta 21041.704 //um, distance between the datum points

//Sensor Geometry, version 3
#define v3_1_cen_y_um 10521.5    //relative distance (y) from top left datum to center of top left sensor, in um
#define v3_1_cen_x_um 8103    //relative distance (x) from top left datum to center of top left sensor, in um
#define v3_sensor_pitch_x 1045 //um
#define v3_sensor_size_x 500 //um
#define v3_sensor_size_y 4000 //um

//Sensor Geometry, version 4
#define v4_1_cen_x_um 8103      //relative distance (y) from top left datum to center of top left sensor, in um
#define v4_1_cen_y_um 9521.2   //relative distance (x) from top left datum to center of top left sensor, in um
#define v4_sensor_pitch_x 1045 //um
#define v4_sensor_pitch_y 2000 //um
#define v4_sensor_size_y 1500 //um
#define v4_sensor_size_x 500 //um

//Hyperspectral Aperture Geometry
#define hs_1_cen_x_um 0      //relative distance (y) from top left datum to center of top left aperture, in um
#define hs_1_cen_y_um 0 //relative distance (x) from top left datum to center of top left aperture, in um
#define hs_pitch_x 622.3 //um
#define hs_pitch_y 575.2 //um

#define REAG_PICKUP_Z 97000
#define SPOT_DRY_TIME 15000 //ms
#define GRID_SIZE 55;

#define Well_X 119000   //Reagent one
#define Well_Y 129200
#define WELL_PITCH 4500

//Height at which robot transfers between reagents and sensor
#define JOG_Z 60000


//Struct used to store a single action in the execution loop
//Note that
struct step {
    int x_pos;
    int y_pos;
    int z_pos;
    int sleepTime;  //sleeps before the move command is issued
};



//Lookup table for fixed reagent coordinates
//first index is reagent #, then (x,y)
const int REAG_COORDS[21][2] = {
    {Well_X,Well_Y},    //1{166000,156400}
    {(Well_X - (1 * WELL_PITCH)),Well_Y},    //2{161500,156400}
    {(Well_X - (2 * WELL_PITCH)),Well_Y},    //3{157000,156400}
    {(Well_X - (3 * WELL_PITCH)),Well_Y},    //4{152500,156400}
    {(Well_X - (4 * WELL_PITCH)),Well_Y},    //5{148000,156400}
    {(Well_X - (5 * WELL_PITCH)),Well_Y},    //6{143500,156400}
    {(Well_X - (6 * WELL_PITCH)),Well_Y},    //7{139000,156400
    {(Well_X - (7 * WELL_PITCH)),Well_Y},    //8{134500,156400}
    {(Well_X - (8 * WELL_PITCH)),Well_Y},    //9{130000,156400
    {(Well_X - (9 * WELL_PITCH)),Well_Y},    //10{125500,156400}
    {(Well_X - (10 * WELL_PITCH)),Well_Y},    //11{121000,156400}
    {(Well_X - (11 * WELL_PITCH)),Well_Y},    //12{116500,156400}
    {(Well_X - (12 * WELL_PITCH)),Well_Y},    //13{112000,156400}
    {(Well_X - (13 * WELL_PITCH)),Well_Y},    //14{107500,156400}
    {(Well_X - (14 * WELL_PITCH)),Well_Y},    //15{103000,156400}
    {(Well_X - (15 * WELL_PITCH)),Well_Y},    // 16{98500,156400}
    {(Well_X - (16 * WELL_PITCH)),Well_Y},    // 17{94000,156400
    {(Well_X - (17 * WELL_PITCH)),Well_Y},    // 18{89500,156400}
    {(Well_X - (18 * WELL_PITCH)),Well_Y},    // 19{85000,156400}
    {(Well_X - (19 * WELL_PITCH)),Well_Y},    // 20{80500,156400
    {(Well_X - (20 * WELL_PITCH)),Well_Y},     //21well 21 for calibration liquid{76000,156400}
};

class SpotControl : public QObject
{
    Q_OBJECT

public:
    SpotControl();
    void rxReagAssignments(int reag[8][15], int reag_v3[8], int reag_v4[2][8], int reag_hs[6][4]);
    void rxSpotConfigV3(int hor, int vert, int overshoot);
    void rxSpotConfigV4(int hor, int vert, int overshoot);
    void rxTestSpots(int spot1, int spot2);

signals:
    void moveToPos(int x, int y, int z);
    void clearError();
    void complete();
    void progress(int percent);
    void startTimer(int sec);

public slots:
    void startSpotting();
    void doneMoving();
    void rxStartCoordinates(int x1,int y1,int x2,int y2,int z);
    void rxScriptLoopCnt(int loopCnt);
    void rxSleepTimerDone();
    void rxSensorVersion(int version);
    void rxRobotError();
    void moveToCoord(int datum1_x1, int datum1_y1, int col, int row);

private:
    int reagAssign[8][15];      //array of each sensor location, and what reagent (indexed) is designated at each location;
    int reagAssign_v3[8];       //array of each sensor location, and what reagent (indexed) is designated at each location (sensor V3)
    int reagAssign_v4[2][8];    //array of each sensor location, and what reagent (indexed) is designated at each location (sensor V4)
    int reagAssign_hs[6][4];    //array of each aperture location, and what reagent (indexed) is designated at each location (hyperspectral aperture)
    int testSpotOne,testSpotTwo;

    bool moving;                          //block further commands until robot is in position
    bool sleeping;
    QList<step> stepList;                 //Queue of steps to execute in main loop
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
    int ZDist;
    int totNumSteps;
    int stepNum;
    int scriptIterations;                 //new feature allowing to loop multiple times through script
    QTimer *sTimer;

    //Calibration vars
    double steps_per_micron;
    int r18c13_cent_x;  //steps
    int r18c13_cent_y;  //steps
    int v3_sensor1_cen_x;  //steps
    int v3_sensor1_cen_y;  //steps
    int v4_r1c1_cen_x;  //steps
    int v4_r1c1_cen_y;  //steps
    int hs_r1c1_cen_x;  //steps
    int hs_r1c1_cen_y;  //steps
    long double theta;

    //Sensor version, determines array pattern
    int sensor_version;

    //Spot configurations for sensor v3 and v4
    int spot_v3_hor, spot_v3_vert, spot_v3_os;
    int spot_v4_hor, spot_v4_vert, spot_v4_os;

    //Utility functions
    void processStep();
    void addTestSpot(int reag, int num);
    void addReagentSensorCoordsSpot_v12(int reag);
    void addReagentSensorCoordsSpot_v3(int reag);
    void addReagentSensorCoordsSpot_v4(int reag);
    void addReagentSensorCoordsSpot_hs(int reag);

};

#endif // SPOTCONTROL_H
