#pragma once

enum Motors
{
	ClawLeft = 1,
	LiftRight1,
	LiftRight2,
	DriveRight1,
	DriveRight2,
	LiftLeft1,
	LiftLeft2,
	DriveLeft1,
	DriveLeft2,
	ClawRight,
};

enum Sensors
{
	LeftQuadTop = 1,
	LeftQuadBot,
	RightQuadTop,
	RightQuadBot,
	ClawPotent = 1, // analogue sensor
	LeftLineTrack = 5,
	RightLineTrack = 6,
};
