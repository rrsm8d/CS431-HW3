#include <vector>
#include <iostream>
using namespace std;

void printSequence(vector<int>& sequence, int& numProcess)
{
	// Make sure not too much was allocated
	if (sequence.size() == numProcess) 
	{
		// If the sequence has the same number of processes, there is no loop, and should not deadlock
		cout << "Safe sequence found: ";
		for (int i = 0; i < numProcess; i++)
		{
			cout << "P" << sequence[i];
			if (i < numProcess - 1) // dont put an arrow at the end
			{
				cout << " -> ";
			}
		}
		cout << endl;
	}
	else 
	{
		cout << "Provided information has a deadlock" << endl;
	}
}

int getNum(string what)
{
	int num = 0;
	while (true)
		{
		cout << "Enter the number of " << what << ": ";
		try {
			cin >> num;
			if (num < 0)
				throw exception ("Number too large");
			break;
		}
		catch (exception& e)
		{
			cerr << e.what() << endl;
			continue;
		}
	}
	return num;
}

void autoTest()
{
	// The following is a copy of what was present on Lecture 7 - Deadlocks.pdf, page 36
	vector<vector<int>> alloc = {
	{0, 1, 0},
	{2, 0, 0},
	{3, 0, 2},
	{2, 1, 1},
	{0, 0, 2}
	};
	vector<vector<int>> max = {
	{7, 5, 3},
	{3, 2, 2},
	{9, 0, 2},
	{2, 2, 2},
	{4, 3, 3}
	};
	vector<int> available = { 3, 3, 2 };
	vector<int> sequence;

	int numProcess = max.size();
	int numResource = max[0].size();
	vector<vector<int>> need(numProcess, vector<int>(numResource));

	// Safety algorithm
	// 1. Initialize work and finish
	vector<bool> finish(numProcess);
	for (int i = 0; i < numProcess; i++)
	{
		finish[i] = false;
	}
	for (int i = 0; i < numProcess; i++)
	{
		for (int j = 0; j < numResource; j++)
		{
			need[i][j] = max[i][j] - alloc[i][j];
		}
	}
	vector<int> work = available;

	// 2. Find i such that both: finish[i] == false and need[i] <= work
	bool condWasSatisfied = true;
	while (condWasSatisfied) // Restart the search every time we find a good process
	{
		condWasSatisfied = false;
		for (int i = 0; i < numProcess; i++)
		{
			if (finish[i] == false && (need[i] <= work))
			{
				// 3. Add up work, record process to sequence, set finished to true
				sequence.push_back(i);
				// Work = work + alloc[i]
				for (int j = 0; j < numResource; j++)
				{
					work[j] += alloc[i][j];
				}
				finish[i] = true;
				// Go back to step 2.
				condWasSatisfied = true;
			}
		}
	}

	// 4. If all are true, safe.
	bool seqIsSafe = true;
	for (int i = 0; i < finish.size(); i++)
	{
		if (finish[i] == false)
		{
			seqIsSafe = false;
		}
	}
	if (seqIsSafe)
	{
		printSequence(sequence, numProcess);
	}
	else cerr << "This has a deadlock!" << endl;
}

void manualTest()
{
	int numProcess = getNum("processes");
	int numResource = getNum("resources");
	vector<vector<int>> alloc;
	vector<vector<int>> max;
	vector<int> available;
	vector<int> sequence;

	cout << endl << "DATA FOR ALLOCATION" << endl;
	for (int i = 0; i < numProcess; i++)
	{
		for (int j = 0; j < numResource; j++)
		{
			cout << "P" << i << ", R" << j << ": ";
			int value;
			cin >> value;
			alloc[i][j] = value;
		}
	}

	cout << endl << "DATA FOR MAX" << endl;
	for (int i = 0; i < numProcess; i++)
	{
		for (int j = 0; j < numResource; j++)
		{
			cout << "P" << i << ", R" << j << ": ";
			int value;
			cin >> value;
			max[i][j] = value;
		}
	}

	cout << endl << "DATA FOR AVAILABLE" << endl;
	for (int i = 0; i < numResource; i++)
	{
		cout << "R" << i << ": ";
		int value;
		cin >> value;
		available[i] = value;
	}

	vector<vector<int>> need(numProcess, vector<int>(numResource));

	// Safety algorithm
	// 1. Initialize work and finish
	vector<bool> finish(numProcess);
	for (int i = 0; i < numProcess; i++)
	{
		finish[i] = false;
	}
	for (int i = 0; i < numProcess; i++)
	{
		for (int j = 0; j < numResource; j++)
		{
			need[i][j] = max[i][j] - alloc[i][j];
		}
	}
	vector<int> work = available;

	// 2. Find i such that both: finish[i] == false and need[i] <= work
	bool condWasSatisfied = true;
	while (condWasSatisfied) // Restart the search every time we find a good process
	{
		condWasSatisfied = false;
		for (int i = 0; i < numProcess; i++)
		{
			if (finish[i] == false && (need[i] <= work))
			{
				// 3. Add up work, record process to sequence, set finished to true
				sequence.push_back(i);
				// Work = work + alloc[i]
				for (int j = 0; j < numResource; j++)
				{
					work[j] += alloc[i][j];
				}
				finish[i] = true;
				// Go back to step 2.
				condWasSatisfied = true;
			}
		}
	}

	// 4. If all are true, safe.
	bool seqIsSafe = true;
	for (int i = 0; i < finish.size(); i++)
	{
		if (finish[i] == false)
		{
			seqIsSafe = false;
		}
	}
	if (seqIsSafe)
	{
		printSequence(sequence, numProcess);
	}
	else cerr << "This has a deadlock!" << endl;
}

int main()
{
	cout << "Would you like to manually input all info, or automatically have everything entered?"
		<< endl << "1. Manually enter info [This may take a long time to do]"
		<< endl << "2. Automatically enter info [Data from Lecture 7 - Deadlocks slides]"
		<< endl;
	int ans;
	cin >> ans;
	switch (ans)
	{
	case 1:
		manualTest();
		break;
	case 2:
		autoTest();
		break;
	default:
		cout << "Invalid. Please enter either 1 or 2" << endl;
		cin >> ans;
		break;
	}
	return 0;
}