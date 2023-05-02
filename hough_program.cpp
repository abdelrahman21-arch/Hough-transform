#include"hough.h"
#include<math.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
const double pi = 3.1415926;
void LineDetect(unsigned char* dst , int* NumberOflines, struct LineParameter* DetectedLine,int height, int width)
{
    int VotingThreshold=100;
    float DiagonalLength = sqrt((float)(height*height+width*width));
	float MinimumAngle = 0;
	float MaximumAngle = 90;
	float AngleInterval_Coarse = 2;
	float AngleInterval_Fine = 0.1;
	float DistanceInterval_Coarse =2;
	float DistanceInterval_Fine =1;
    int NumberofAngles_Coarse = (int)(MaximumAngle/AngleInterval_Coarse)+1;
	int NumberofDistances_Coarse = (int)DiagonalLength/DistanceInterval_Coarse+1;
    unsigned int** number_bins_array = (unsigned int**)malloc( NumberofAngles_Coarse*sizeof(unsigned int*));
    for(int i=0; i< NumberofAngles_Coarse;i++)
	{
		number_bins_array[i] = (unsigned int*)malloc(NumberofDistances_Coarse*sizeof(unsigned int));
	}
    for(int i=0; i<NumberofAngles_Coarse;i++)
	{
		for(int j=0; j<NumberofDistances_Coarse;j++)
		{
			number_bins_array[i][j]=0;
		}
	}
    HoughTransform_Line(dst, MinimumAngle, AngleInterval_Coarse, NumberofAngles_Coarse, 0, DistanceInterval_Coarse, NumberofDistances_Coarse, number_bins_array, height, width); 
	
	int MaxVote=110;
	int M=0;
	int N=0; //  VoteTable_Coarse[M][N] has the maximum votes. 

	while (MaxVote>VotingThreshold)
	{
		FindMaxVote(number_bins_array,NumberofAngles_Coarse, NumberofDistances_Coarse, &M, &N);  
		MaxVote = number_bins_array[M][N];
		float Angle_withMaxVote_Coarse = MinimumAngle+M*AngleInterval_Coarse;
		float Distance_withMaxVote_Coarse = N*DistanceInterval_Coarse;
		(*NumberOflines)++;
		DetectedLine[(*NumberOflines)-1].angle = Angle_withMaxVote_Coarse;
		DetectedLine[(*NumberOflines)-1].distance=Distance_withMaxVote_Coarse;
		cout<<Angle_withMaxVote_Coarse<<" "<<Distance_withMaxVote_Coarse<<" "<<MaxVote<<endl;

		number_bins_array[M][N]=0;

		if(N>0){
			number_bins_array[M][N-1]=0;
			if(M>0){
				number_bins_array[M-1][N-1]=0;
			}
			if(M<NumberofAngles_Coarse-1){
				number_bins_array[M+1][N-1]=0;
			}
		}

		if(N<NumberofDistances_Coarse-1){
			number_bins_array[M][N+1]=0;
			if(M>0){
				number_bins_array[M-1][N+1]=0;
			}
			if(M<NumberofAngles_Coarse-1){
				number_bins_array[M+1][N+1]=0;
			}
		}

		if(M>0)
		{
			number_bins_array[M-1][N]=0;
		}

		if(M<NumberofAngles_Coarse-1)
		{
			number_bins_array[M+1][N]=0;
		}

		
	}
	
			
	


	for(int i=0; i< NumberofAngles_Coarse;i++)
	{
		free(number_bins_array[i]);
	}

	free(number_bins_array);


}
void HoughTransform_Line(unsigned char* dst, float MinAngle, float AngleInterval, int angle_inc, float MinDistance,  float DistanceInterval, int distance_inc,unsigned int** number_bins_array, int height, int width )
{
	for (int x=0; x<width; x++)
	{
		for(int y=0; y<height; y++)
		{
			if(*(dst+y*width+x)==255)
			{
				for (int i=0; i<angle_inc; i++)
				{
					float Angle = MinAngle+AngleInterval*((float)i);
					float Distance = ((float)x)*cos(Angle/180*pi)+((float)y)*sin(Angle/180*pi);
					int j = (int)((Distance-MinDistance)/DistanceInterval);
					if(j < distance_inc)
						{
							number_bins_array[i][j]=((number_bins_array[i][j])+1);
						}

				}
			}
		}
	}
}
void FindMaxVote(unsigned int** number_bins_array, int angle_inc, int distance_inc, int* M, int* N)
{
	unsigned int *MaxVote_for_Diff_Angles = (unsigned int*)malloc(angle_inc*sizeof(unsigned int));

	for(int i=0; i<angle_inc; i++)
	{
		int j = FindArrayMax(number_bins_array[i], distance_inc); 
		MaxVote_for_Diff_Angles[i] = number_bins_array[i][j];
	}

	*M = FindArrayMax(MaxVote_for_Diff_Angles, angle_inc);
	*N = FindArrayMax(number_bins_array[*M], distance_inc);

	free(MaxVote_for_Diff_Angles);
}


int FindArrayMax(unsigned int* Array, int No_of_Element)
{
	int N=0; //N is used to indicate the order of the element int the array which has the max value: Array[N] is the maximum in Array
	for(int i=1; i<No_of_Element; i++)
	{
		N = Larger_in_Array(Array, N, i);
	}

	return N;

}




int Larger_in_Array(unsigned int* Array, int x, int y)
{
	if(Array[x]>=Array[y])
		return x;
	else 
		return y;
}





