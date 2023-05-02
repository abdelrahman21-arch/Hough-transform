struct LinepolarRepresentation {
    float distance;
    float angle;
};
void LineDetect(unsigned char* dst , int* NumberOflines, struct LinepolarRepresentation* DetectedLine,int height, int width);
void HoughTransform_Line(unsigned char* dst, float MinAngle, float AngleInterval, int angle_inc, float MinDistance,  float DistanceInterval, int distance_inc, unsigned int** number_bins_array, int nHeight, int nWidth );
void FindMaxVote(unsigned int** number_bins_array, int angle_inc, int distance_inc, int* M, int* N);   // To get the *M and *N,  number_bins_array[*M][*N] has the maximum vote number

int FindArrayMax(unsigned int* Array, int No_of_Element); //return int n that Array[n] has the maximum value is this array

int Larger_in_Array(unsigned int* Array, int x, int y);