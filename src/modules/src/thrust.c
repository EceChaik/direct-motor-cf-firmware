#include "FreeRTOS.h"
#include "task.h"
#include "crtp.h"
#include "thrust.h"
#include "motors.h" // Contains function motorsSetRatio

struct thrustCrtpValues {
	uint16_t thrust;
	int motorID;
} __attribute__((packed));

static bool isInit;
struct thrustCrtpValues targetValue;

static void thrustCrtpCB(CRTPPacket* pk);
static uint16_t limitThrust(int32_t value);


void thrustInit(void)
{

	if(isInit)
		return;
		
	crtpInit();
	
	crtpRegisterPortCB(CRTP_PORT_THRUST, thrustCrtpCB);

	isInit = true;

}


static void thrustCrtpCB(CRTPPacket* pk)
{
	targetValue = *((struct thrustCrtpValues*)pk->data);
	uint16_t motorRatio = limitThrust(targetValue.thrust);
	if (targetValue.motorID == 4) {
		motorsSetRatio(MOTOR_M1, motorRatio);
		motorsSetRatio(MOTOR_M2, motorRatio);
		motorsSetRatio(MOTOR_M3, motorRatio);
		motorsSetRatio(MOTOR_M4, motorRatio);
	} else {
		motorsSetRatio(targetValue.motorID, motorRatio);
	}
}


static uint16_t limitThrust(int32_t value)
{
	if(value > UINT16_MAX)
	{
		value = UINT16_MAX;
	}
	else if(value < 0)
	{
		value = 0;
	}
	return (uint16_t)value;
}

