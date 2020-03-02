#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fstream>

using namespace std;

struct Course {
	string courseName;
	int enrollNum;
	string instructor;
	string building;
	string roomNum;
	int roomSize;
	int timeSlot;
	void print(); //prints out the course information
	void operator = (Course); //creates an = operator to transfer data into a course
};

const int numCourses = 12; //holds the total number of courses
const int numRooms = 7; //holds the number of rooms
const int numInstructors = 6; //holds the number of instructors
const int numTimeSlots = 7;  //holds the number of available time slots

/*Create the arrays to hold all of the data to be used for the classes*/
//list of courses and their respective enrollment numbers in the same positions on each array
array<string, numCourses> courseNames = { "CS 101 A", "CS 101 B", "CS 201 A", "CS 201 B", "CS 191 A", "CS 191 B", "CS 291 B", "CS 291 A", "CS 303", "CS 341", "CS 449", "CS 461" };
array<int, numCourses> enrollNums     = {  40,         25,         30,         30,         60,         20,         40,         20,         50,       40,       55,       40 };

//list of instructors
array<string, numInstructors> instructors = { "Hare", "Bingham", "Kuhail", "Mitchell", "Rao", "Staff" };

//rooms and capacitites, each has same array position for all three values
array<string, numRooms> buildings = { "Haag", "Haag", "Royall", "Katz", "Flarsheim", "Flarsheim", "Bloch" };
array<string, numRooms> roomNums  = { "301",  "206",  "204",    "209",  "310",       "260",       "0009" };
array<int, numRooms> roomSizes    = {  70,     30,     70,       50,     80,          25,          30 };

//list of all time slots as ints in military time
array<int, numTimeSlots> timeSlots = { 1000, 1100, 1200, 1300, 1400, 1500, 1600 };


int assessFit(array<Course, numCourses>); //takes a schedule and returns its fitness score
Course changeSchedule(array<Course, numCourses>, int); //takes a schedule and makes one random change to the selected course then returns the course

int main() {
	//intitialze random seed
	srand(time(NULL));
	int randomNum;
	int attempts = 0;//holds the number of attempted changes (successful or unsuccessful)
	int successes = 0;//holds the number of successful changes
	int totalLoops = 0;//test variable to see how many loops it does!!!!!!!!!!!!
	bool finished = false;//holds the finished flag to stop attempting to change the schedule
	int fit;//holds the fitness score of the schedule
	Course temp;
	float temperature = 99;



	//create an array of the size of the number of courses and fill it with the appropriate course names and enrollment numbers and fill the remaining values with random choices
	array<Course, numCourses> courses;
	for (int i = 0; i < numCourses; i++) {
		//fill course name and enrollment numbers with respective data
		courses[i].courseName = courseNames[i];
		courses[i].enrollNum = enrollNums[i];

		//sets a random instructor for the course
		randomNum = rand() % numInstructors;
		courses[i].instructor = instructors[randomNum];

		//sets random room for the course with its respective building and capacity
		randomNum = rand() % numRooms;
		courses[i].building = buildings[randomNum];
		courses[i].roomNum = roomNums[randomNum];
		courses[i].roomSize = roomSizes[randomNum];

		//sets random timeslot for the course
		randomNum = rand() % numTimeSlots;
		courses[i].timeSlot = timeSlots[randomNum];
	}

	//runs a in a loop until it loops 4000 times in a row without making a change
	while (!finished) {
		//find the fitness score of the schedule
		fit = assessFit(courses);

		//find which course to change
		randomNum = rand() % numCourses;

		//hold the old course info incase the change is not accepted
		temp = courses[randomNum];

		//change the course
		courses[randomNum] = changeSchedule(courses, randomNum);

		//compare fitness of new schedule, if better than accept, else accept it based on probability or reject it
		if (assessFit(courses) > fit || (rand() % 100) < temperature) {
			successes++;
		}
		else {
			courses[randomNum] = temp;//change the course back
		}
		attempts++;
		totalLoops++;

		//check if finished(attempts == 4000 && successes == 0)
		if (attempts == 4000 && successes == 0)
			finished = true;

		//check if temperature needs to be reduced, either successes == 400 or attempts == 4000, lower temperature if true and reset counters
		if (attempts == 4000 || successes == 400) {
			successes = 0;
			attempts = 0;
			temperature = temperature * 0.9;
		}
	}

	//create file stream to add the fitness score after outputting the schedule.
	ofstream output;
	
	//clear the output text file
	output.open("output.txt", ofstream::out | ofstream::trunc);
	output.close();

	//open the text file to append to it
	output.open("output.txt", ofstream::out | ofstream::app);
	//print the final schedule out
	for (int i = 0; i < numCourses; i++) {
		courses[i].print();
	}
	output << "Fitness Score of Schedule: " << fit << endl;
	output.close();

}

//prints the information of a course
void Course::print() {
	ofstream output;
	output.open("output.txt", ofstream::out | ofstream::app);

	output << "Course Name: " << courseName << endl;
	output << "Instructor:  " << instructor << endl;
	output << "Room:        " << building << " " << roomNum << endl;
	output << "Time:        ";
	if (timeSlot == 1200)
		output << timeSlot / 100 << " PM";
	else if ((timeSlot / 1200) < 1) 
		output << timeSlot/100 << " AM";
	else output << (timeSlot/100) % 12 << " PM";
	output << endl << endl;

	output.close();
}

//creates an = operator to transfer data into a course
void Course::operator = (Course course) {
	courseName = course.courseName;
	enrollNum = course.enrollNum;
	instructor = course.instructor;
	building = course.building;
	roomNum = course.roomNum;
	roomSize = course.roomSize;
	timeSlot = course.timeSlot;
}

//takes a schedule and makes one random change to the selected course
Course changeSchedule(array<Course, numCourses> courses, int randomCourseNum) {
	int randomType;//will hold what is getting changed (instructor, room, or time)
	int randomChoice;//will hold what the type is being changed to
	
	//initialize random seed
	srand(time(NULL));

	//chooses randomly whether to change the instructor (0), room (1), or time (2)
	randomType = rand() % 3;

	//instructor change
	if (randomType == 0) { 
		randomChoice = rand() % numInstructors;
		courses[randomCourseNum].instructor = instructors[randomChoice];
	}
	//room change
	else if (randomType == 1) {
		randomChoice = rand() % numRooms;
		courses[randomCourseNum].building = buildings[randomChoice];
		courses[randomCourseNum].roomNum = roomNums[randomChoice];
		courses[randomCourseNum].roomSize = roomSizes[randomChoice];
	}
	//time change
	else if (randomType == 2) {
		randomChoice = rand() % numTimeSlots;
		courses[randomCourseNum].timeSlot = timeSlots[randomChoice];
	}
	return courses[randomCourseNum];
}

//takes a schedule and returns its fitness score
int assessFit(array<Course, numCourses> courses) {
	int fitness = 0;
	bool roomOpen = true;
	bool instructorOpen = true;

	//make variables to keep count of how many courses each instructor has
	int hare = 0;
	int bingham = 0;
	int kuhail = 0;
	int mitchell = 0;
	int rao = 0;

	//add points if cs101a and cs101b are at least 3 hours apart
	if (abs(courses[0].timeSlot - courses[1].timeSlot) >= 300)
		fitness += 5;

	//add points if cs191a and cs191b are at least 3 hours apart
	if (abs(courses[4].timeSlot - courses[5].timeSlot) >= 300)
		fitness += 5;


	//create array with positions of 101, 191, 101, and 291 classes
	int arr[] = { 0,4,1,5,2,7,3,6 };
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i += 2) {
		//subtract points for cs x01 and x91 at the same times
		if (courses[arr[i]].timeSlot == courses[arr[i+1]].timeSlot)
			fitness -= 15;

		//add points for adjacent cs x01 and x91 times
		if (abs(courses[arr[i]].timeSlot - courses[arr[i+1]].timeSlot) == 100) {
			fitness += 5;

			//add points if in the same building
			if (courses[arr[i]].building.compare(courses[arr[i + 1]].building) == 0)
				fitness += 5;
			//subtract points if cs x01 is in katz or bloch
			else if (courses[arr[i]].building.compare(buildings[3]) == 0 || courses[arr[i]].building.compare(buildings[6]) == 0) {
				fitness -= 3;
				//subtract more points if cs x91 is in katz or bloch
				if (courses[arr[i + 1]].building.compare(buildings[3]) == 0 || courses[arr[i + 1]].building.compare(buildings[6]) == 0)
					fitness -= 3;
			}
			//subtract points if cs x91 is in katz or bloch
			else if (courses[arr[i + 1]].building.compare(buildings[3]) == 0 || courses[arr[i + 1]].building.compare(buildings[6]) == 0) {
				fitness -= 3;
			}
		}
	}


	for (int i = 0; i < numCourses; i++) {
		//course is taught by staff
		if (courses[i].instructor.compare(instructors[5]) == 0)
			fitness++;

		//room is large enough for course
		if (courses[i].enrollNum < courses[i].roomSize) {
			fitness += 5;
			if (courses[i].enrollNum >= courses[i].roomSize / 2)
				fitness += 2;
		}

		//add 1 to each instructor for each course they are teaching
		if (courses[i].instructor.compare(instructors[0]) == 0) {
			hare++;
			//course is taught by Hare and he can teach it
			if (i <= 3 || (i >= 6 && i <= 8) || i >= 10)
				fitness += 3;
		}
		if (courses[i].instructor.compare(instructors[1]) == 0) {
			bingham++;
			//course is taught by Bingham and he can teach it
			if (i <= 7 || i == 10)
				fitness += 3;
		}
		if (courses[i].instructor.compare(instructors[2]) == 0) {
			kuhail++;
			//course is taught by Kuhail and he can teach it
			if (i == 8 || i == 9)
				fitness += 3;
		}
		if (courses[i].instructor.compare(instructors[3]) == 0) {
			mitchell++;
			//course is taught by Mitchell and he can teach it
			if (i <= 9 && i >= 4)
				fitness += 3;
		}
		if (courses[i].instructor.compare(instructors[4]) == 0) {
			rao++;
			//course is taught by Rao and he can teach it
			if (i >= 6 && i != 10)
				fitness += 3;
		}

		for (int j = 0; j < numCourses; j++) {
			//course does not have conflicting room and time
			if (i != j && courses[i].roomNum == courses[j].roomNum && courses[i].timeSlot == courses[j].timeSlot)
				roomOpen = false;

			//check if instructor does not have another class at that time false if they do
			if (i != j && courses[i].instructor.compare(courses[j].instructor) == 0 && courses[i].timeSlot == courses[j].timeSlot)
				instructorOpen = false;
		}

		//course does not have conflicting room and time
		if (roomOpen)
			fitness += 5;

		//course instructor is not teaching another course at the same time
		if (instructorOpen)
			fitness += 5;

		instructorOpen = true;
		roomOpen = true;
	}
	
	//subtract points for each instructor with over 4 courses
	if (hare > 4)
		fitness -= 5 * (hare - 4);
	if (bingham > 4)
		fitness -= 5 * (bingham - 4);
	if (kuhail > 4)
		fitness -= 5 * (kuhail - 4);
	if (mitchell > 4)
		fitness -= 5 * (mitchell - 4);
	if (rao > 4)
		fitness -= 5 * (rao - 4);

	//subtract points if Rao or Mitchell have more courses than Hare or Bingham
	if (mitchell > hare || mitchell > bingham)
		fitness -= 10;
	if (rao > hare || rao > mitchell)
		fitness -= 10;

	return fitness;
}