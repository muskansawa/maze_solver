#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char** Maze;
int** Visited;
int Rows;
int Columns;
int R_Start;
int C_Start;

enum Maze_Layout
{
	Space,
	Wall,
	End,
	BackTrack,
	Trace
};

void Alloc_Visited()
{
	Visited = malloc(Rows * sizeof(int*));
	int i;
	for(i=0; i<Rows; i++)
	{
		Visited[i] = malloc(Columns * sizeof(int*));
	}
}

void Alloc_Maze()
{
	Maze = malloc(Rows * sizeof(char*));
	int i;
	for(i=0; i<Rows; i++)
	{
		Maze[i] = malloc(Columns * sizeof(char*));
	}
}

void Get_Maze(char* SampleMaze)
{
	char c;
	char T_Rows[3] = {'\0'};
	char T_Columns[3] = {'\0'};
	int Get_Rows = 0;
	int Get_Columns = 0;
	int Flag = 0;
	
	FILE* Maze_File = fopen(SampleMaze, "r");
	
	if(Maze_File)
	{
		while((c = getc(Maze_File)) != EOF)
		{
			if(c == '\n')
			{
				break;
			}
			else if(c == ',')
			{
				Flag = 1;
			}
			else if(!Flag)
			{
				T_Rows[Get_Rows] = c;
				Get_Rows++;
			}
			else
			{
				T_Columns[Get_Columns] = c;
				Get_Columns++;
			}
		}
	}
	
	Rows = atoi(T_Rows);
	Columns = atoi(T_Columns) + 1;
	
	Alloc_Maze();
	
	int i,j;
	for(i=0; i<Rows; i++)
	{
		for(j=0; j<Columns; j++)
		{
			if((c = getc(Maze_File)) == '\n')
			{
				c = getc(Maze_File);
			}
			Maze[i][j] = c;
			if(c == 'S')
			{
				R_Start = i;
				C_Start = j;
			}
		}
	}
	fclose(Maze_File);
}

void Initialize_Visited()
{
	Alloc_Visited();
	int i, j;
	for (i=0; i<Rows; i++)
	{
		for (j=0; j<Columns; j++)
		{
			if (Maze[i][j] == '+')
			{
				Visited[i][j] = Wall;
			}
			else if (Maze[i][j] == 'E')
			{
				Visited[i][j] = End;
			}
			else
			{
				Visited[i][j] = Space;
			}
		}
	}
}

int DFS(int row, int column)
{
	int* current = &Visited[row][column];

	if(*current == End)
	{
		return 1;
	}

	if(*current == Space)
	{
		*current = BackTrack;

		if (DFS(row, column - 1))
		{
			*current = Trace;
			return 1;
		} 
		if (DFS(row + 1, column))
		{
			*current = Trace;
			return 1;
		} 
		if (DFS(row, column + 1))
		{
			*current = Trace;
			return 1;		
		} 
		if (DFS(row - 1, column))
		{
			*current = Trace;
			return 1;
		}
	}
	return 0;
}

void Add_Trace(int Final)
{
	int i, j;
	for (i = 0; i < Rows; i++)
	{
		for (j = 0; j < Columns; j++)
		{
			if (Maze[i][j] != 'S' && Visited[i][j] == Trace)
			{
				Maze[i][j] = '.';
			}
			else if (Maze[i][j] != 'S' && Visited[i][j] == BackTrack && Final == 0)
			{
				Maze[i][j] = '/';
			}
			else if (Maze[i][j] != 'S' && Visited[i][j] == BackTrack && Final == 1)
			{
				Maze[i][j] = ' ';
			}
		}
	}
}

void Print_Maze()
{
	for(int i=0; i<Rows; i++)
	{
		for(int j=0; j<Columns; j++)
		{
			printf(" %c",Maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void Print_Visited()
{
	for(int i=0; i<Rows; i++)
	{
		for(int j=0; j<Columns-1; j++)
		{
			printf("%d ",Visited[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void Delay(int number_of_seconds) 
{
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds);
}

void Step_Print()
{
	system("clear");
	Add_Trace(0);
	Print_Maze();
	Delay(200);
}

int Maze_Main(int row, int column)
{
	int i,j;
	Step_Print();
	int* current = &Visited[row][column];
	if(*current == End)
	{
		return 1;
	}
	if(*current == Space)
	{
		*current = Trace;
		for(i = -1;i <= 1;i++)
		{
			for(j = -1; j<= 1;j++)
			{
				if(i != j && i != -j)
				{
					if(Maze_Main(row + i, column + j))
					{
						return 1;
					}
				}
			}
		}
		*current = BackTrack;
	}
	return 0;
}

int main()
{
	Get_Maze("Trial_Maze2.txt");
	system("clear");
	Print_Maze();
	Initialize_Visited();
	Print_Visited();
	Delay(5000);
	//DFS(R_Start,C_Start);
	if(Maze_Main(R_Start,C_Start))
	{
		Add_Trace(1);
		Delay(1000);
		printf("Final Route: \n");
		Delay(2000);
		Print_Maze();
	}
	//Add_Trace();
	Print_Visited();
	return 0;
}
