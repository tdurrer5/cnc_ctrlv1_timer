/*This file is part of the Maslow Control Software.
    The Maslow Control Software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Maslow Control Software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with the Maslow Control Software.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2017 Bar Smith*/

// This contains all of the Motion commands

#ifndef motion_h
#define motion_h

// These are used for movement tracking and need to be available to the ISR
extern volatile bool movementUpdated;
#if misloopDebug > 0
  extern volatile bool  inMovementLoop;
  extern volatile bool  movementFail;
#endif

//tdu init

  // Serial variables
  #define INCBUFFERLENGTH 128 // The number of bytes(characters) allocated to the
                              // incoming buffer.
  #define EXPGCODELINE 60     // Maximum expected Gcode line length in characters
                              // including line ending character(s).  Assumes
                              // client will not send more than this.  Ground
                              // Control is currently set to 60.  NIST spec allows
                              // 256. This value must be <= INCBUFFERLENGTH
  #define MAXBUFFERLINES 4    // The maximum number of lines allowed in the buffer
  #define POSITIONTIMEOUT 200 // The minimum number of milliseconds between
                              // position reports sent to Ground Control.  This
                              // cannot be larger than the connection timout in
                              // Ground Control which is 2000, a smaller number
                              // takes more processing time for sending data
                              // a larger number make position updates in GC less
                              // smooth.  This is only a minimum, and the actual
                              // timeout could be significantly larger.

  #define LOOPINTERVAL 10000 // What is the frequency of the PID loop in microseconds
  // Convenience Defines - Maybe move into a nuts and bolts file?
  #define FORWARD           1
  #define BACKWARD         -1
  #define CLOCKWISE        -1
  #define COUNTERCLOCKWISE  1
  #define MILLIMETERS 1
  #define INCHES      25.4

#define sq(x)  x * x


  // Storage for global system states
  // Some of this could be more appropriately moved to the gcode parser
  typedef struct
  {
    int stop;                  // Stop flag.
    int state;                 // State tracking flag
    int pause;                 // Pause flag.
    float xPosition;            // Cartessian position of XY axes
    float yPosition;            // Cached because calculating position is intensive
    float zPosition;            // Cached because calculating position is intensive
    float steps[3];             // Encoder position of axes
    bool  useRelativeUnits;     //
    unsigned long lastSerialRcvd; // The millis of the last rcvd serial command, used by watchdo
    int   lastGCommand;         //Stores the value of the last command run eg: G01 -> 1
    int   lastTool;             //Stores the value of the last tool number eg: T4 -> 4
    int   nextTool;             //Stores the value of the next tool number eg: T4 -> 4
    float inchesToMMConversion; //Used to track whether to convert from inches, can probably be done in a way that doesn't require RAM
    float feedrate;             //The feedrate of the machine in mm/min
//    bool writeStepsToEEPROM;    // Flag to determine when need to write encoder steps to EEPROM.. used in execSystemRealtime and axis.attach
    // THE FOLLOWING IS USED FOR IMPORTING SETTINGS FROM FIRMWARE v1.00 AND EARLIER
    // It can be deleted at some point
//    byte oldSettingsFlag;
  } system_t;

  extern system_t sys;

  typedef struct {  // I think this is about ~128 bytes in size if I counted correctly

    unsigned int maxFeed;
    float maxZRPM;

/*
    float machineWidth;
    float machineHeight;
    float distBetweenMotors;
    float motorOffsetY;
    float sledWidth;
    float sledHeight;
    float sledCG;
    byte kinematicsType;
    float rotationDiskRadius;
    unsigned int axisDetachTime;
    unsigned int chainLength;
    unsigned int originalChainLength;
    float encoderSteps;
    float distPerRot;
    bool zAxisAttached;
  //  SpindleAutomationType spindleAutomateType;
    float zDistPerRot;
    float zEncoderSteps;
    float KpPos;
    float KiPos;
    float KdPos;
    float propWeightPos;
    float KpV;
    float KiV;
    float KdV;
    float propWeightV;
    float zKpPos;
    float zKiPos;
    float zKdPos;
    float zPropWeightPos;
    float zKpV;
    float zKiV;
    float zKdV;
    float zPropWeightV;
    float chainSagCorrection;
    byte chainOverSprocket;
    byte fPWM;
    float leftChainTolerance;
    float rightChainTolerance;
    float positionErrorLimit;
    float zAxisUpperLimit; // A safety feature to prevent motors from attempting to move the z axis beyong a user specified constraint.
    float zAxisLowerLimit;
    byte eepromValidData;  // This should always be last, that way if an error
                           // happens in writing, it will not be written and we
*/

  } settings_t;            // will know to reset the settings
  extern settings_t sysSettings;

  extern Axis leftAxis;
  extern Axis rightAxis;
  extern Axis zAxis;

  extern Kinematics kinematics;

 // extern useZ;


// end tdu

void initMotion();
int   coordinatedMove(const float&, const float&, const float&, float);
void  singleAxisMove(Axis*, const float&, const float&);
int   arc(const float&, const float&, const float&, const float&, const float&, const float&, const float&, const float&, const float&, const float&);
float calculateFeedrate(const float&, const float&);
float computeStepSize(const float&);
void movementUpdate();
void motionDetachIfIdle();

float constrain(float, int, int);

void execSystemRealtime(void);

void  G1(const String&, int);

#endif
