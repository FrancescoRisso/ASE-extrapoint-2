#include "directions.h"


directions DIR_getPerpendicular(directions dir, bool primary) {
	directions resPrimary;

	switch(dir) {
		case DIR_down: resPrimary = DIR_left; break;
		case DIR_up: resPrimary = DIR_left; break;
		case DIR_right: resPrimary = DIR_up; break;
		case DIR_left: resPrimary = DIR_up; break;
		case DIR_down_left: resPrimary = DIR_up_left; break;
		case DIR_up_right: resPrimary = DIR_up_left; break;
		case DIR_up_left: resPrimary = DIR_up_right; break;
		case DIR_down_right: resPrimary = DIR_up_right; break;
	}

	if(primary) return resPrimary;
	return DIR_opposite(resPrimary);
}


directions DIR_sum(directions dir1, directions dir2) {
	switch(dir1) {
		case DIR_up:
			if(dir2 == DIR_left) return DIR_up_left;
			if(dir2 == DIR_right) return DIR_up_right;
			break;
		case DIR_down:
			if(dir2 == DIR_left) return DIR_down_left;
			if(dir2 == DIR_right) return DIR_down_right;
			break;
		case DIR_left:
			if(dir2 == DIR_up) return DIR_up_left;
			if(dir2 == DIR_down) return DIR_down_left;
			break;
		case DIR_right:
			if(dir2 == DIR_up) return DIR_up_right;
			if(dir2 == DIR_down) return DIR_down_right;
			break;
		case DIR_down_left:
			if(dir2 == DIR_right) return DIR_down;
			if(dir2 == DIR_up) return DIR_left;
			break;
		case DIR_down_right:
			if(dir2 == DIR_left) return DIR_down;
			if(dir2 == DIR_up) return DIR_right;
			break;
		case DIR_up_right:
			if(dir2 == DIR_left) return DIR_up;
			if(dir2 == DIR_down) return DIR_right;
			break;
		case DIR_up_left:
			if(dir2 == DIR_right) return DIR_up;
			if(dir2 == DIR_down) return DIR_left;
			break;
		case DIR_none: return dir2;
	}

	if(dir2 == DIR_none) return dir1;
	return DIR_none;
}


directions DIR_opposite(directions dir) {
	switch(dir) {
		case DIR_down: return DIR_up;
		case DIR_up: return DIR_down;
		case DIR_left: return DIR_right;
		case DIR_right: return DIR_left;
		case DIR_down_left: return DIR_up_right;
		case DIR_down_right: return DIR_up_left;
		case DIR_up_left: return DIR_down_right;
		case DIR_up_right: return DIR_down_left;
		default: return DIR_none;
	}
}


directions DIR_computeFromPoints(int startR, int startC, int endR, int endC) {
	directions horiz = DIR_none, vert = DIR_none;

	if(abs(startR - endR) > 1) return DIR_none;
	if(abs(startC - endC) > 1) return DIR_none;

	if(startR > endR) vert = DIR_up;
	if(startR < endR) vert = DIR_down;

	if(startC > endC) vert = DIR_left;
	if(startC < endC) vert = DIR_right;

	return DIR_sum(horiz, vert);
}
