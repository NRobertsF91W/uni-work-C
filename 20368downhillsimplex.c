#include <stdio.h> //We're going to need to print things later
#include <math.h> //always need maths, pow, sqrt, all that good stuff


double F(double x0,double x1) // Pi is an array with Pi[0] = x0 and Pi[1] = x1 are arguments of the rosenbrock function
{
	double y;
	return y=(100*pow(x1- pow(x0, 2.),2.))+pow((1-x0),2.); //rosenbrock eqn
}

//double W(double x0, double x1, double y) //function to print out data into a nice list
//{
//	FILE *fp;
//	fp = fopen("rosenbrock.txt","a");
//	fprintf(fp,"\r\n%lf, %lf, %lf",x0,x1,y); //\r is used as \n didn't properly format the text in a way i liked
//	fclose(fp);
//}

//main()
//{
//	double x0;double x1; double y; // main function here just loops over the values that are needed, it calls the two functions for calculating y and printing to a file
//	for(x0=-2;x0<2.04;x0=x0+0.04){ //goes to 2.04 to ensure it gets the full range
//		x1=1;
//		y=F(x0,x1);
//		W(x0,x1,y);
//		printf("%lf		%lf		%lf\n",x0,x1,y);
//	}
//	return 0;
//} //This is the end of the program used to print the slice of the rosenbrock function, Function F is used in both programs so i didn't comment it out.

double *mid(double *Pll,double *Pii,double *Pbarr) //function to find mid point, all the functions from here till main take pointers and return pointers as a way to use arrays within functions without any changes being lost.
{
	*Pbarr=(*Pll + *Pii)/2.0;
	*(Pbarr+1)=(*(Pll+1) + *(Pii+1))/2.0;
	return Pbarr;
}

double *ref(double *Pbarr, double *Phh, double *Preff) //function to reflect test point 
{
	*Preff=(2*(*Pbarr) - *Phh);
	*(Preff+1)=(2*(*(Pbarr+1)) - *(Phh+1));
	return Preff;
}

double *expand(double *Preff,double *Pbarr,double *Pexpp) //function to expand test point
{
	*Pexpp= (2*(*Preff) - *Pbarr);
	*(Pexpp+1)= (2*(*(Preff+1)) - *(Pbarr+1));
	return Pexpp;
}

double *con(double *Phh,double *Pbarr, double *Pconn) //function to contract test point
{
	*Pconn= (*Phh + *Pbarr)/2;
	*(Pconn+1)= (*(Phh+1) + *(Pbarr+1))/2;
	return Pconn;
}

double *replaceall(double *Phh,double *Pll,double *Pii) //replace all vertices
{
	*Pii=(*Pii + *Pll)/2;
	*(Pii+1)=(*(Pii+1) + *(Pll+1))/2;
	*Phh=(*Phh + *Pll)/2;
	*(Phh+1)=(*(Phh+1) + *(Pll+1))/2;
	return Phh,Pii; 
}

double *horl(double *P00,double *P11,double *P22,double *Phh,double *Pll,double *Pii) //function to determine which point is max, mid, min. (horl)= high or low. This function uses a lot of variables as it was written out before I realised you needed to use pointers, it could probably be slimmed down a bit but it all works at the moment.
{
	double y0; double y1; double y2;
	double Phx0;double Phx1; double Plx0;double Plx1;
	double Pix0;double Pix1;double P0x0;double P0x1;
	double P1x0;double P1x1;double P2x0;double P2x1; //defining variables for later
	P0x0 = *P00;P0x1 = *(P00+1);P1x0 = *P11;P1x1 = *(P11+1); P2x0 = *P22; P2x1 = *(P22+1);
	y0=F(P0x0,P0x1);
	y1=F(P1x0,P1x1);
	y2=F(P2x0,P2x1); //calling the rosenbrock function to find y's
	if (y0>y1 || y0>y2) // for each of the y values this set of if 
	{					// statements determines it's relation to the 
		if (y0>y1 && y0>y2) //other values
		{
			*Phh=P0x0;
			*(Phh+1)=P0x1; //if y0 is the max the Ph[] array is updated
		}
		else
		{
			*Pii=P0x0;
			*(Pii+1)=P0x1; //if y0 is the mid Pi[] is updated
		}
	}
	else
	{
		*Pll=P0x0;
		*(Pll+1)=P0x1; //if y0 is the min Pl[] is updated
	} 
	if (y1>y0 || y1>y2)
	{
		if (y1>y0 && y1>y2)
		{
			*Phh=P1x0;
			*(Phh+1)=P1x1; //if y1 is the max Ph[] is updated
		}
		else
		{
			*Pii=P1x0;
			*(Pii+1)=P1x1; //if y1 is the mid Pi[] is updated
		}
	}
	else
	{
		*Pll=P1x0;
		*(Pll+1)=P1x1; //if y1 is the min Pl[] is updated
	} 
	if (y2>y1 || y2>y0)
	{
		if (y2>y1 && y2>y0)
		{
			*Phh=P2x0;
			*(Phh+1)=P2x1; //if y2 is the max Ph[] is updated
		}
		else
		{
			*Pii=P2x0;
			*(Pii+1)=P2x1; //if y2 is the mid Pi[] is updated
		}
	}
	else
	{
		*Pll=P2x0;
		*(Pll+1)=P2x1; //if y2 is the min Pl[] is updated
	}  //If statements seemed like the obvious way to determine which point was higher or lower. 
	return Phh,Pll,Pii; //function returns a pointer to the memory address of the Ph[],Pl[],Pi[] arrays as arrays can't be passed through functions.
}


void main ()
{
	double P0[]={0.0,0.0};double P1[]={2.0,0.0};double P2[]={0.0,2.0}; //set original vertices, coordinates are written as arrays to keep them together. 
	double Pl[1];double Ph[1];double Pi[1];double Pbar[1];
	double Pref[1];double Pexp[1];double Pcon[1];double *P00;
	double *P11;double *P22; double *Phh; double *Pll;
	double *Pii;double *Pbarr;double *Preff; double *Pexpp;
	double *Pconn; float error;float a; float b;
	int c;double yl;double yh; double yi; 
	double yref; double yexp; double ycon; double ybar;//establish variables - pointers have repeated letters at the end to differentiate them. I liked the look of four declarations on each line here, 
	P00=P0; P11=P1; P22=P2; Phh=Ph; Pll=Pl; Pii=Pi; Pbarr=Pbar; Preff=Pref; Pexpp=Pexp; Pconn=Pcon; //define pointers to each of the arrays to be passed through functions
	error = 1; //established an error value so the while loop runs properly at first, this is changed on each run through of the loop
	c=0; //counter so the algorithm is only ran 1000 times max
	horl(P00,P11,P22,Phh,Pll,Pii); //find which point is high, mid, low
	printf("high point = %lf,%lf, low point = %lf,%lf, mid point = %lf,%lf\n", *Phh,*(Phh+1),*Pll,*(Pll+1),*Pii,*(Pii+1));//prints midpoints to make sure the function is working, this was originally in as a test to make sure this part was working before i added the rest but i think it's useful to see the starting vertices.
	do //this is the main while loop that runs through the algorithm defined by the flow chart. It calls the previously defined functions to transform the shape of the simplex.
	{
		mid(Pll,Pii,Pbarr); //find mid point of mid and low point
		ref(Pbarr,Phh,Preff); // reflect high point across the mid point
		yl=F(Pl[0],Pl[1]);
		yh=F(Ph[0],Ph[1]);
		yi=F(Pi[0],Pi[1]); //calculating y's again
		yref=F(Pref[0],Pref[1]);
		
			if (yref<yl) //this is my interpretation of the flow chart given in the question, it allows the simplex to slowly creep to the lowest point it can find. If it were to be generalised to n dimensions you would need for loops to replace values in the arrays, I haven't done this as in 2 dimensions it is clearer without. 
			{
				expand(Preff,Pbarr,Pexpp);
				yexp=F(Pexp[0],Pexp[1]);
				if (yexp<yl)
				{
					Ph[0]=Pexp[0];
					Ph[1]=Pexp[1];
				}
				else
				{
					Ph[0]=Pref[0];
					Ph[1]=Pref[1];
				}
			}
			else if (yref>yi)
			{
				if (yref>yh)
				{
					con(Phh,Pbarr,Pconn);
					ycon=F(Pcon[0],Pcon[1]);
					if (ycon>yh)
					{
						replaceall(Phh,Pll,Pii);
					}
					else
					{
						Ph[0]=Pcon[0];
						Ph[1]=Pcon[1];
					}
				}
				else
				{
					Ph[0]=Pref[0];
					Ph[1]=Pref[1];
					con(Phh,Pbarr,Pconn);
					ycon=F(Pcon[0],Pcon[1]);
					if (ycon>yh)
					{
						replaceall(Phh,Pll,Pii);
					}
					else
					{
						Ph[0]=Pcon[0];
						Ph[1]=Pcon[1];
					}
				}
			}
			else
			{
				Ph[0]=Pref[0];
				Ph[1]=Pref[1];
			}
		
		horl(Phh,Pll,Pii,Phh,Pll,Pii); //horl is called at the end of the loop so the correct values are used in the next loop and error calculations
		mid(Pll,Pii,Pbarr); //these calculations are needed for the error
		ref(Pbarr,Phh,Preff);
		yl=F(Pl[0],Pl[1]);
		yh=F(Ph[0],Ph[1]);
		yi=F(Pi[0],Pi[1]);
		ybar=F(Pbar[0],Pbar[1]);
		a=((pow((yl-ybar),2.)/2.) + (pow((yh-ybar),2.)/2.) + (pow((yi-ybar),2.)/2.)); //first part of the error calculation, probably could have done it all in one step but i split it into two to make it easier to spot any mistakes
		b=sqrt(a);
		error=b;
		c++; //Add one to the counter, little shoutout to c++
		printf("c = %d\n ",c); // Print the counter number to show how many times the algorithm has been iterated
	
	} while (error>=0.00000001 && c<1000); //end condition of the while loop, error <10^-8 or 1000 iterations
	printf("high point = %lf,%lf, low point = %lf,%lf, mid point = %lf,%lf max y = %lf, mid y = %lf, min y = %lf, with error = %e \n", *Phh,*(Phh+1),*Pll,*(Pll+1),*Pii,*(Pii+1),yh,yi,yl,error); //once the condition has been reached all the information needs to be printed, 
}

	
	
	

	
