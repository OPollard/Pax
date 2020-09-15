// Copyright of Codebird Ltd 2020 - inclusive © 
#pragma once

//Pax
#define YOUNG_WALK_SPEED 400.0f
#define ADULT_WALK_SPEED 300.0f //250.0f
#define OLD_WALK_SPEED 200.0f  //100.0f
#define STANDARD_ROTATION_SPEED 540.0f

//Pax Variables
#define SEAT_SNAP_RANGE 50.0f
//Set in Pax_BP
#define INFLUENCE_RANGE 100.0f //90.0f

//Pax State
#define NUTRITION_CHX_YNG -0.75f
#define ENERGY_CHX_YNG -0.25f 
#define EXCREMENT_CHX_YNG 1.25f 

#define NUTRITION_CHX_AVG -1.25f // 2.0f
#define ENERGY_CHX_AVG -0.75f //-0.5f
#define EXCREMENT_CHX_AVG 0.25f // 0.25f

#define NUTRITION_CHX_OLD -0.25f
#define ENERGY_CHX_OLD -1.25f 
#define EXCREMENT_CHX_OLD 0.75f 

#define SOCIAL_CHX_AVG 0.0f

//Gameplay Timings
#define SLEEP_TIME 10.0f
#define SERVICE_RECHARGE_TIME 20.0f //30.0f

//Starting Money
#define START_MONEY_POOR 1250.0f
#define START_MONEY_AVG	2500.0f
#define START_MONEY_WEALTHY	5000.0f
#define START_MONEY_RICH 10000.0f

// Money - ref Pax State
#define CASH_DROP 100.0f
#define FIRSTCLASS_RATIO 0.9f
#define BUSINESS_RATIO 0.6f
#define ECONOMY_RATIO 0.3f

// Level Times
#define FREEFLIGHT_WARNINGTIME 30.0f
#define FREEFLIGHT_BOARDINGTIME 120.0f
#define FREEFLIGHT_CRUISETIME 300.0f 

// Penalties
#define DEATH_PENALTY -2500.0f

//Ranks - not accurate
#define BEGINNER_CAP 50000.0
#define INTERMEDIATE_CAP 150000.0
#define EXPERT_CAP 325000.0
#define SUPERHERO_CAP 1000000.0