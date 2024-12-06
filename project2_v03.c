/* project2_v03.c
 * Teal Hoffman
 * EE 160 
 * 12/02/24
 * This is the source code for project2 
 *********************************************************************************************************************************
 * PURPOSE
 * -------
 * The purpose of this coding project is to use the least squares method and Gauss Jordan method to determine a best fit 
 *    equation based on what the user picks as their highest degree of that polynomial. The user will also input the file name 
 *    they would like to use for the x data and y data, and the file name for the output resulting data. 
 * 
 *********************************************************************************************************************************
 * VERSIONS
 * --------
 * Version 1: Takes data from the test files and show what was taken. 
 * Version 2: Takes the data and does the calculations for the least squares method and put it into a matrix 'A' and matrix 'B' 
 *    then it will put it into an augmented matrix. Most of the structure is done in this version. 
 * Version 3: Asks the user if they would like to write an output file, and the code that implements the guass jordan method
 *     will now have k=2, k=3, and k=4. I will also calculate the y calc from the x data and the determined a values. 
 * 
 *********************************************************************************************************************************
 * PSEUDOCODE
 * ----------
 * 1)  Declare variables (See below for each of the descriptions), declare file pointers (one for reading and another for writing)
 *           Also declare arrays used for the Gauss Elimination process. Make an array with type 'polynomial'
 * 2)  Prompt the user and obtain input for the data file name and order of the polynomial they would like to fit (variable ‘k’). 
 *           If they input something other than 1-4, give them an error message. If the file name input is invalid then give 
 *           them an error message as well.
 * 3)  Open the data file and save data into a user-defined structure containing x-data and y-data. 
 * 4)  Use a loop and if statments that will calculate the best fit polynomial based on what the user enters 
 * 5)  Within each loop you will have this structure: 
 *           5.1) Initialize the sums, then calculate them and assign to the matrix A and B (if k=1 then it will be a 2x2 and so on..)
 *           5.2) Show matrix A and B then combine them into an augmented matrix. Show the augmented matrix
 *           5.3) Do the Gauss Jordan method on the augmented matrix. If k=1 you will have 4 steps, if k=2 you will have 9 steps
 *                     if k=3 you will have 16 steps, and if you have k=4 you will have 25 steps. 
 *           5.4) Show the reduced row matrix and the best fit equation
 *           5.5) Calculate the y calc value using the x data and the 'a' value (The 'a' value is just the B matrix section in the augmented matrix)
 *           5.6) End the loop 
 * 6)  Ask the user if they would like to save the resulting data into another output file. If they do, ask them what they
 *           would like to name the outfile file.
 * 7)  If the user said they would like to save the results to an output file then make to format of the output file and print
 *           it out onto the output file with the created data structure. If done successfully, tell the user that the output 
 *           was saved to 'name_of_output_file'
 * 8)  Also make a loop with if statements to print out the best fit equation depending on what k is 
 * 
 *********************************************************************************************************************************
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#define TRUE 1
#define FALSE 0

/* MAIN FUNCTION */
int main (void) {
    /* Declare variables */
    int i, j;               // Used for matrix loops 
    char fname[100];        // The file name the user will choose to use the data 
    char outputfile[100];   // The file name the user will enter for writing the results to an output file 
    char line[256];         // Buffer to store each line
    int c = 0;              // c will become the number of data points (N)
    int k;                  // k is the degree of the polynomial the user will enter
    double sumx, sumx2, sumx3, sumx4, sumx5, sumx6, sumx7, sumx8;   // The A matrix calculations
    double sumy, sumxy, sumx2y, sumx3y, sumx4y;                     // The B matrix calculations
    int done = FALSE;       // Used for the while !done loop 
    char choice;            // Used if the user wants to store their data in an output file

    /* Declare arrays that will be used for Gauss Jordan */
    double A[20][20];
    double B[20][20];
    double C[20][20];

    /* Create file pointers */
    FILE *fp;   // Used for reading files
    FILE *fw;   // Used for writing files 

    /* Create a struct and make it type 'polynomial' */
    struct polynomial {
        double xdata[100];
        double ydata[100];
        double ycalc[100];
    };

    /* Name the type polynomial 'D'*/
    struct polynomial D;

    /* Ask the user to enter the order of the polynomial they'd like to fit */
    /* If the user entered an integer less than 1 or greater than 4 give an error message */
    printf("Enter the order of the polynomial to fit (integer from 1 to 4): ");
    scanf("%i", &k);
    if ( k <= 0 || k >= 5 ) {
        printf("***ERROR: Please enter an integer from 1 to 4");
    }
    printf("\n");

    /* Ask User to enter the file name they wish to read */
    printf("please enter the file you want to read: ");
    scanf("%s", fname);
    
    /* Attepmy to open file */
    fp = fopen(fname, "r");   // "r" is read only 
    if (fp == NULL) {
        printf("Error reading file\n");
        return 1;
    }
    // else printf("\nfile successfully opened...\n");

    /* Read the file */
    while (fscanf(fp, "%lf,%lf", &D.xdata[c], &D.ydata[c]) == 2) {
           c++;
    }
    printf("\n");
    
    /* Print data temporarily stored in Array A[] 
    printf("Data stored is:\n");
    for (i=0; i<c; i++) {
        printf("%.3lf,%.3lf  \n", D.xdata[i], D.ydata[i]);
    }
    */

    /* Tell the user the amount of data points (N) */
    printf("\n");
    printf("Number of data points (N): %d\n\n", c);

    /* Close file and exit */
    // printf("\nClosing file...\n\n");
    fclose(fp);


    /* Loop for every 'k' value */
    while (!done) {



        /* If the user entered 1 do the 2x2 Gauss Jordan method */
        if ( k == 1 ) {
            /* Initialize sums */
            sumx = 0;
            sumx2 = 0;
            sumy = 0;
            sumxy = 0;
            /* Make the least squares calculations */
            for (i=0; i<c; i++) {
                sumx += D.xdata[i];                // Sum of all the x data 
                sumx2 += D.xdata[i] * D.xdata[i];  // Square of x data 
                sumy += D.ydata[i];                // Sum of all the y data 
                sumxy += D.xdata[i] * D.ydata[i];  // Product of x and y data
            }
            /* Show the results to check for errors */
            //printf("Sum of x (sumx): %.3lf\n", sumx);
            //printf("Sum of x^2 (sumx2): %.3lf\n", sumx2);
            //printf("Sum of y (sumy): %.3lf\n", sumy);
            //printf("Sum of x*y (sumxy): %.3lf\n", sumxy);

            /* Put the sums into an A matrix */
            A[0][0] = c;
            A[1][0] = sumx;
            A[0][1] = sumx;
            A[1][1] = sumx2;
            /* Put the y sums into a B matrix */
            B[0][0] = sumy;
            B[1][0] = sumxy;

            /* Show the A matrix */
            printf("Matrix A:\n");
            for (i=0; i<2; i++) {
                for (j=0; j<2; j++) {
                printf(" %4.1f ", A[i][j]);
                }
            printf("\n");
            }
            /* Show the B matrix */
            printf("Matrix B:\n");
            for (i=0; i<2; i++) {
                for (j=0; j<1; j++) {
                printf(" %4.1f ", B[i][j]);
                }
            printf("\n");
            }
            printf("\n");

            /* Make the augmented matrix C */
            for (i=0; i<2; i++) {
                for (j=0; j<2; j++) {
                    C[i][j] = A[i][j];   
                }
                C[i][2] = B[i][0];       // For every row of C, the last column of C will belong to that specific row of B
            }
            /* Show augmented matrix */
            printf("Augmented Matrix:\n");
            for (i=0; i<2; i++) {
                for (j=0; j<3; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
                printf("\n");
            }

            /* Do the Gauss Jordan Method */
            /* Calculate row 1 = row 1 / c11 */
            double c11 = C[0][0];
            for (j=0; j<3; j++) {
                C[0][j] = C[0][j] / c11;
            }
  
            /* Print this intermediate step 1 
            printf("step 1: \n");
            for (i=0; i<2; i++) {
                for (j=0; j<3; j++) {
                    printf(" %4.1f ", C[i][j]);
                }
                printf("\n");
            } 
            */
  
            /* Caculate row2 = row2 - c21*row1 */
            double c21 = C[1][0];
            for (j=0; j<3; j++) {
                C[1][j] = C[1][j] - (c21* C[0][j]);   
            }
  
            /* Print this intermediate step 2 
            printf("step 2: \n");
            for (i=0; i<2; i++) {
                for (j = 0; j < 3; j++) {
                    printf(" %4.1f ", C[i][j]);
                }
            printf("\n");
            }
            */
  
            /* Calculate row2 = row2 / c22 */
            double c22 = C[1][1];
            for (j=0; j<3; j++) {
                C[1][j] = C[1][j] / c22;
            }
  
            /* Print this intermediate step 3 
            printf("step 3: \n");
            for (i=0; i<2; i++) {
                for (j=0; j<3; j++) {
                    printf(" %4.1f ", C[i][j]);
                }
            printf("\n");
            }
            */

            /* Calculate row1 = row1 - c12*row2 */
            float c12 = C[0][1];
            for (j=0; j<3; j++) {
                C[0][j] = C[0][j] - (c12 * C[1][j]);
            }

            /* Print this intermediate step 4 
            printf("step 4: \n");
            for (i=0; i<2; i++) {
                for (j=0; j<3; j++) {
                    printf(" %4.1f ", C[i][j]);
                }
                printf("\n");
            } 
            */

            /* Show the reduced row matrix */
            printf("\nThe row reduced (RREF) augmented matrix is: \n");
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            }

            printf("\nThe linear least-squares best fit to data is: \ny = %.5lf * x^0 + %.5lf * x^1\n\n", C[0][2], C[1][2]);

            /* Determine what the y calculations are */
            for (i = 0; i < c; i++) {
                D.ycalc[i] = C[0][2] + C[1][2] * D.xdata[i];
            }
            done = TRUE;
        }



        if ( k == 2 ) {
            /* Initialize sums */
            sumx = 0;
            sumx2 = 0;
            sumx3 = 0;
            sumx4 = 0;
            sumy = 0;
            sumxy = 0;
            sumx2y = 0;
            /* Make the least squares calculations */
            for (i=0; i<c; i++) {
                sumx += D.xdata[i];                // Sum of all the x data 
                sumx2 += D.xdata[i] * D.xdata[i];  // Square of x data 
                sumx3 += pow(D.xdata[i], 3);
                sumx4 += pow(D.xdata[i], 4);
                sumy += D.ydata[i];                // Sum of all the y data 
                sumxy += D.xdata[i] * D.ydata[i];  // Product of x and y data
                sumx2y += pow(D.xdata[i], 2) * D.ydata[i];
            }
            /* Put the sums into an A matrix */
            A[0][0] = c;
            A[1][0] = sumx;
            A[0][1] = sumx;
            A[1][1] = sumx2;
            A[2][0] = sumx2;
            A[2][1] = sumx3;
            A[2][2] = sumx4;
            A[0][2] = sumx2;
            A[1][2] = sumx3;
            /* Put the y sums into a B matrix */
            B[0][0] = sumy;
            B[1][0] = sumxy;
            B[2][0] = sumx2y;

            /* Show the A matrix */
            printf("Matrix A:\n");
            for (i=0; i<3; i++) {
                for (j=0; j<3; j++) {
                printf(" %4.1f ", A[i][j]);
                }
            printf("\n");
            }
            /* Show the B matrix */
            printf("Matrix B:\n");
            for (i=0; i<3; i++) {
                for (j=0; j<1; j++) {
                printf(" %4.1f ", B[i][j]);
                }
            printf("\n");
            }
            printf("\n");

            /* Make the augmented matrix C */
            for (i=0; i<3; i++) {
                for (j=0; j<3; j++) {
                    C[i][j] = A[i][j];   
                }
                C[i][3] = B[i][0];       // For every row of C, the last column of C will belong to that specific row of B
            }
            /* Show augmented matrix */
            printf("Augmented Matrix:\n");
            for (i=0; i<3; i++) {
                for (j=0; j<4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
                printf("\n");
            }

            /* Do the Gauss Jordan Method */
            /* Step 1 is to divide row 1 with c11 */
            float c11 = C[0][0];         // c11 is the top left position of the matrix 
            for (j=0; j<4; j++) {
                C[0][j] = C[0][j] / c11;
            }

            /* Print this intermediate step 1 
            printf("step 1: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 2 is row 2 = row 2 minus c21*row1 */
            float c21 = C[1][0];
            for (j=0; j<4; j++) {
                C[1][j] = C[1][j] - (c21 * C[0][j]);
            }

            /* Print this intermediate step 2 
            printf("step 2: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */ 

            /* Step 3 is row 3 = row 3 - c31*row1 */
            float c31 = C[2][0];
            for (j=0; j<4; j++) {
                C[2][j] = C[2][j] - (c31 * C[0][j]);
            }

            /* Print this intermediate step 3 
            printf("step 3: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 4 is row 2 = row 2 / c22 */
            float c22 = C[1][1];
            for (j=0; j<4; j++) {
                C[1][j] = C[1][j] / c22;
            }

            /* Print this intermediate step 4 
            printf("step 4: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 5 is row 3 = row 3 - c32*row2 */
            float c32 = C[2][1];
            for (j=0; j<4; j++) {
                C[2][j] = C[2][j] - (c32 * C[1][j]);
            }

            /* Print this intermediate step 5 
            printf("step 5: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 6 is row 1 = row 1 - c12*row2 */
            float c12 = C[0][1];
            for (j=0; j<4; j++) {
                C[0][j] = C[0][j] - (c12 * C[1][j]);
            }

            /* Print this intermediate step 6 
            printf("step 6: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 7 is row 3 = row 3 / c33 */
            float c33 = C[2][2];
            for (j=0; j<4; j++) {
                C[2][j] = C[2][j] / c33;
            }

            /* Print this intermediate step 7 
            printf("step 7: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 8 is row 2 = row 2 - c23*row3 */
            float c23 = C[1][2];
            for (j=0; j<4; j++) {
                C[1][j] = C[1][j] - (c23 * C[2][j]);
            }

            /* Print this intermediate step 8 
            printf("step 8: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            } 
            */

            /* Step 9 is row 1 = row 1 - c13*row3 */
            float c13 = C[0][2];
            for (j=0; j<4; j++) {
                C[0][j] = C[0][j] - (c13 * C[2][j]);
            }

            /* Print this intermediate step 9 
            printf("step 9: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            }
            */

            /* Show the reduced row matrix */
            printf("\nThe row reduced (RREF) augmented matrix is: \n");
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 4; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            }

            printf("\nThe best-fit least squares equation is: \n");
            printf("%.5lf * x^0 + %.5lf * x^1 + %.5lf * x^2\n\n", C[0][3], C[1][3], C[2][3]);

            /* Determine what the y calculations are */
            for (i = 0; i < c; i++) {
                D.ycalc[i] = C[0][3] + C[1][3] * D.xdata[i] + (C[2][3] * pow(D.xdata[i], 2));
            }

            done = TRUE;
        }



        if ( k == 3 ) {
            /* Initialize sums */
            sumx = 0;
            sumx2 = 0;
            sumx3 = 0;
            sumx4 = 0;
            sumx5 = 0;
            sumx6 = 0;
            sumy = 0;
            sumxy = 0;
            sumx2y = 0;
            sumx3y = 0;
            /* Make the least squares calculations */
            for (i=0; i<c; i++) {
                sumx += D.xdata[i];                // Sum of all the x data 
                sumx2 += D.xdata[i] * D.xdata[i];  // Square of x data 
                sumx3 += pow(D.xdata[i], 3);
                sumx4 += pow(D.xdata[i], 4);
                sumx5 += pow(D.xdata[i], 5);
                sumx6 += pow(D.xdata[i], 6);
                sumy += D.ydata[i];                // Sum of all the y data 
                sumxy += D.xdata[i] * D.ydata[i];  // Product of x and y data
                sumx2y += pow(D.xdata[i], 2) * D.ydata[i];
                sumx3y += pow(D.xdata[i], 3) * D.ydata[i];
            }
            /* Put the sums into an A matrix */
            A[0][0] = c;
            A[1][0] = sumx;
            A[0][1] = sumx;
            A[1][1] = sumx2;
            A[2][0] = sumx2;
            A[2][1] = sumx3;
            A[2][2] = sumx4;
            A[0][2] = sumx2;
            A[1][2] = sumx3;
            A[3][0] = sumx3;
            A[3][1] = sumx4;
            A[3][2] = sumx5;
            A[3][3] = sumx6;
            A[0][3] = sumx3;
            A[1][3] = sumx4;
            A[2][3] = sumx5;

            /* Put the y sums into a B matrix */
            B[0][0] = sumy;
            B[1][0] = sumxy;
            B[2][0] = sumx2y;
            B[3][0] = sumx3y;

            /* Show the A matrix */
            printf("Matrix A:\n");
            for (i=0; i<4; i++) {
                for (j=0; j<4; j++) {
                printf(" %4.1f ", A[i][j]);
                }
            printf("\n");
            }
            /* Show the B matrix */
            printf("Matrix B:\n");
            for (i=0; i<4; i++) {
                for (j=0; j<1; j++) {
                printf(" %4.1f ", B[i][j]);
                }
            printf("\n");
            }
            printf("\n");

            /* Make the augmented matrix C */
            for (i=0; i<4; i++) {
                for (j=0; j<4; j++) {
                    C[i][j] = A[i][j];   
                }
                C[i][4] = B[i][0];       // For every row of C, the last column of C will belong to that specific row of B
            }
            /* Show augmented matrix */
            printf("Augmented Matrix:\n");
            for (i=0; i<4; i++) {
                for (j=0; j<5; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
                printf("\n");
            }

            /* Do the Gauss Jordan Method */
            /* Step 1 is to divide row 1 with c11 */
            float c11 = C[0][0];         // c11 is the top left position of the matrix 
            for (j=0; j<5; j++) {
                C[0][j] = C[0][j] / c11;
            }

            /* Step 2 is row 2 = row 2 minus c21*row1 */
            float c21 = C[1][0];
            for (j=0; j<5; j++) {
                C[1][j] = C[1][j] - (c21 * C[0][j]);
            }

            /* Step 3 is row 3 = row 3 - c31*row1 */
            float c31 = C[2][0];
            for (j=0; j<5; j++) {
                C[2][j] = C[2][j] - (c31 * C[0][j]);
            }

            /* Step 4 is row 4 = row 4 - c41*row1 */
            float c41 = C[3][0];
            for (j=0; j<5; j++) {
                C[3][j] = C[3][j] - (c41 * C[0][j]);
            }

            /* Step 5 is row 2 = row 2 / c22 */
            float c22 = C[1][1];
            for (j=0; j<5; j++) {
                C[1][j] = C[1][j] / c22;
            }

            /* Step 6 is row 4 = row 4 - c42*row2 */
            float c42 = C[3][1];
            for (j=0; j<5; j++) {
                C[3][j] = C[3][j] - (c42 * C[1][j]);
            }

            /* Step 7 is row 3 = row 3 - c32*row2 */
            float c32 = C[2][1];
            for (j=0; j<5; j++) {
                C[2][j] = C[2][j] - (c32 * C[1][j]);
            }

            /* Step 8 is row 1 = row 1 - c12*row2 */
            float c12 = C[0][1];
            for (j=0; j<5; j++) {
                C[0][j] = C[0][j] - (c12 * C[1][j]);
            }

            /* Step 9 is row 3 = row 3 / c33 */
            float c33 = C[2][2];
            for (j=0; j<5; j++) {
                C[2][j] = C[2][j] / c33;
            }

            /* Step 10 is row 4 = row 4 - c43*row3 */
            float c43 = C[3][2];
            for (j=0; j<5; j++) {
                C[3][j] = C[3][j] - (c43 * C[2][j]);
            }

            /* Step 11 is row 2 = row 2 - c23*row3 */
            float c23 = C[1][2];
            for (j=0; j<5; j++) {
                C[1][j] = C[1][j] - (c23 * C[2][j]);
            }

            /* Step 12 is row 1 = row 1 - c13*row3 */
            float c13 = C[0][2];
            for (j=0; j<5; j++) {
                C[0][j] = C[0][j] - (c13 * C[2][j]);
            }

            /* Step 13 is row 4 = row 4 / c44 */
            float c44 = C[3][3];
            for (j=0; j<5; j++) {
                C[3][j] = C[3][j] / c44;
            }

            /* Step 14 is row 3 = row 3 - c34*row4 */
            float c34 = C[2][3];
            for (j=0; j<5; j++) {
                C[2][j] = C[2][j] - (c34 * C[3][j]);
            }

            /* Step 15 is row 2 = row 2 - c24*row4 */
            float c24 = C[1][3];
            for (j=0; j<5; j++) {
                C[1][j] = C[1][j] - (c24 * C[3][j]);
            }

            /* Step 16 is row 1 = row 1 - c14*row4 */
            float c14 = C[0][3];
            for (j=0; j<5; j++) {
                C[0][j] = C[0][j] - (c14 * C[3][j]);
            }

            /* Show the reduced row matrix */
            printf("\nThe row reduced (RREF) augmented matrix is: \n");
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 5; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            }

            printf("\nThe best-fit least squares equation is: \n");
            printf("%.5lf * x^0 + %.5lf * x^1 + %.5lf * x^2 + %.5lf * x^3\n\n", C[0][4], C[1][4], C[2][4], C[3][4]);

            /* Determine what the y calculations are */
            for (i = 0; i < c; i++) {
                D.ycalc[i] = C[0][4] + (C[1][4] * D.xdata[i]) + (C[2][4] * pow(D.xdata[i], 2)) + (C[3][4] * pow(D.xdata[i], 3));
            }
            done = TRUE;
        }



        if ( k == 4 ) {
            /* Initialize sums */
            sumx = 0;
            sumx2 = 0;
            sumx3 = 0;
            sumx4 = 0;
            sumx5 = 0;
            sumx6 = 0;
            sumx7 = 0;
            sumx8 = 0;
            sumy = 0;
            sumxy = 0;
            sumx2y = 0;
            sumx3y = 0;
            sumx4y = 0;
            /* Make the least squares calculations */
            for (i=0; i<c; i++) {
                sumx += D.xdata[i];                // Sum of all the x data 
                sumx2 += D.xdata[i] * D.xdata[i];  // Square of x data 
                sumx3 += pow(D.xdata[i], 3);
                sumx4 += pow(D.xdata[i], 4);
                sumx5 += pow(D.xdata[i], 5);
                sumx6 += pow(D.xdata[i], 6);
                sumx7 += pow(D.xdata[i], 7);
                sumx8 += pow(D.xdata[i], 8);
                sumy += D.ydata[i];                // Sum of all the y data 
                sumxy += D.xdata[i] * D.ydata[i];  // Product of x and y data
                sumx2y += pow(D.xdata[i], 2) * D.ydata[i];
                sumx3y += pow(D.xdata[i], 3) * D.ydata[i];
                sumx4y += pow(D.xdata[i], 4) * D.ydata[i]; 
            }
            /* Put the sums into an A matrix */
            A[0][0] = c;
            A[1][0] = sumx;
            A[0][1] = sumx;
            A[1][1] = sumx2;
            A[2][0] = sumx2;
            A[2][1] = sumx3;
            A[2][2] = sumx4;
            A[0][2] = sumx2;
            A[1][2] = sumx3;
            A[3][0] = sumx3;
            A[3][1] = sumx4;
            A[3][2] = sumx5;
            A[3][3] = sumx6;
            A[0][3] = sumx3;
            A[1][3] = sumx4;
            A[2][3] = sumx5;
            A[4][0] = sumx4;
            A[4][1] = sumx5;
            A[4][2] = sumx6;
            A[4][3] = sumx7;
            A[4][4] = sumx8;
            A[0][4] = sumx4;
            A[1][4] = sumx5;
            A[2][4] = sumx6;
            A[3][4] = sumx7;

            /* Put the y sums into a B matrix */
            B[0][0] = sumy;
            B[1][0] = sumxy;
            B[2][0] = sumx2y;
            B[3][0] = sumx3y;
            B[4][0] = sumx4y;

            /* Show the A matrix */
            printf("Matrix A:\n");
            for (i=0; i<5; i++) {
                for (j=0; j<5; j++) {
                printf(" %4.1f ", A[i][j]);
                }
            printf("\n");
            }
            /* Show the B matrix */
            printf("Matrix B:\n");
            for (i=0; i<5; i++) {
                for (j=0; j<1; j++) {
                printf(" %4.1f ", B[i][j]);
                }
            printf("\n");
            }
            printf("\n");

            /* Make the augmented matrix C */
            for (i=0; i<5; i++) {
                for (j=0; j<5; j++) {
                    C[i][j] = A[i][j];   
                }
                C[i][5] = B[i][0];       // For every row of C, the last column of C will belong to that specific row of B
            }
            /* Show augmented matrix */
            printf("Augmented Matrix:\n");
            for (i=0; i<5; i++) {
                for (j=0; j<6; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
                printf("\n");
            }

            /* Do the Gauss Jordan Method for k = 4*/
            /* Step 1 is to divide row 1 with c11 */
            float c11 = C[0][0];         // c11 is the top left position of the matrix 
            for (j=0; j<6; j++) {
                C[0][j] = C[0][j] / c11;
            }

            /* Step 2 is row 2 = row 2 minus c21*row1 */
            float c21 = C[1][0];
            for (j=0; j<6; j++) {
                C[1][j] = C[1][j] - (c21 * C[0][j]);
            }

            /* Step 3 is row 3 = row 3 - c31*row1 */
            float c31 = C[2][0];
            for (j=0; j<6; j++) {
                C[2][j] = C[2][j] - (c31 * C[0][j]);
            }

            /* Step 4 is row 4 = row 4 - c41*row1 */
            float c41 = C[3][0];
            for (j=0; j<6; j++) {
                C[3][j] = C[3][j] - (c41 * C[0][j]);
            }

            /* Step 5 is row 5 = row 5 - c51*row1 */
            float c51 = C[4][0];
            for (j=0; j<6; j++) {
                C[4][j] = C[4][j] - (c51 * C[0][j]);
            }

            /* Step 6 is row 2 = row 2 / c22 */
            float c22 = C[1][1];
            for (j=0; j<6; j++) {
                C[1][j] = C[1][j] / c22;
            }

            /* Step 7 is row 5 = row 5 - c52*row2 */
            float c52 = C[4][1];
            for (j=0; j<6; j++) {
                C[4][j] = C[4][j] - (c52 * C[1][j]);
            }

            /* Step 8 is row 4 = row 4 - c42*row2 */
            float c42 = C[3][1];
            for (j=0; j<6; j++) {
                C[3][j] = C[3][j] - (c42 * C[1][j]);
            }

            /* Step 9 is row 3 = row 3 - c32*row2 */
            float c32 = C[2][1];
            for (j=0; j<6; j++) {
                C[2][j] = C[2][j] - (c32 * C[1][j]);
            }

            /* Step 10 is row 1 = row 1 - c12*row2 */
            float c12 = C[0][1];
            for (j=0; j<6; j++) {
                C[0][j] = C[0][j] - (c12 * C[1][j]);
            }

            /* Step 11 is row 3 = row 3 / c33 */
            float c33 = C[2][2];
            for (j=0; j<6; j++) {
                C[2][j] = C[2][j] / c33;
            }

            /* Step 12 is row 5 = row 5 - c53*row3 */
            float c53 = C[4][2];
            for (j=0; j<6; j++) {
                C[4][j] = C[4][j] - (c53 * C[2][j]);
            }
            
            /* Step 13 is row 4 = row 4 - c43*row3 */
            float c43 = C[3][2];
            for (j=0; j<6; j++) {
                C[3][j] = C[3][j] - (c43 * C[2][j]);
            }

            /* Step 14 is row 2 = row 2 - c23*row3 */
            float c23 = C[1][2];
            for (j=0; j<6; j++) {
                C[1][j] = C[1][j] - (c23 * C[2][j]);
            }

            /* Step 15 is row 1 = row 1 - c13*row3 */
            float c13 = C[0][2];
            for (j=0; j<6; j++) {
                C[0][j] = C[0][j] - (c13 * C[2][j]);
            }

            /* Step 16 is row 4 = row 4 / c44 */
            float c44 = C[3][3];
            for (j=0; j<6; j++) {
                C[3][j] = C[3][j] / c44;
            }

            /* Step 17 is row 5 = row 5 - c54*row4 */
            float c54 = C[4][3];
            for (j=0; j<6; j++) {
                C[4][j] = C[4][j] - (c54 * C[3][j]);
            }

            /* Step 18 is row 3 = row 3 - c34*row4 */
            float c34 = C[2][3];
            for (j=0; j<6; j++) {
                C[2][j] = C[2][j] - (c34 * C[3][j]);
            }

            /* Step 19 is row 2 = row 2 - c24*row4 */
            float c24 = C[1][3];
            for (j=0; j<6; j++) {
                C[1][j] = C[1][j] - (c24 * C[3][j]);
            }

            /* Step 20 is row 1 = row 1 - c14*row4 */
            float c14 = C[0][3];
            for (j=0; j<6; j++) {
                C[0][j] = C[0][j] - (c14 * C[3][j]);
            }

            /* Step 21 is row 5 = row 5 / c55 */
            float c55 = C[4][4];
            for (j=0; j<6; j++) {
                C[4][j] = C[4][j] / c55;
            }

            /* Step 22 is row 4 = row 4 - c45*row5 */
            float c45 = C[3][4];
            for (j=0; j<6; j++) {
                C[3][j] = C[3][j] - (c45 * C[4][j]);
            }

            /* Step 23 is row 3 = row 3 - c35*row5 */
            float c35 = C[2][4];
            for (j=0; j<6; j++) {
                C[2][j] = C[2][j] - (c35 * C[4][j]);
            }

            /* Step 24 is row 2 = row 2 - c25*row5 */
            float c25 = C[1][4];
            for (j=0; j<6; j++) {
                C[1][j] = C[1][j] - (c25 * C[4][j]);
            }

            /* Step 25 is row 1 = row 1 - c15*row5 */
            float c15 = C[0][4];
            for (j=0; j<6; j++) {
                C[0][j] = C[0][j] - (c15 * C[4][j]);
            }

            /* Show the reduced row matrix */
            printf("\nThe row reduced (RREF) augmented matrix is: \n");
            for (i = 0; i < 5; i++) {
                for (j = 0; j < 6; j++) {
                    printf(" %6.2f ", C[i][j]);
                }
            printf("\n");
            }

            printf("\nThe best-fit least squares equation is: \n");
            printf("y = %.5lf * x^0 + %.5lf * x^1 + %.5lf * x^2 + %.5lf * x^3 + %.5lf * x^4\n\n", C[0][5], C[1][5], C[2][5], C[3][5], C[4][5]);

            /* Determine what the y calculations are */
            for (i = 0; i < c; i++) {
                D.ycalc[i] = C[0][5] + (C[1][5] * D.xdata[i]) + (C[2][5] * pow(D.xdata[i], 2)) + (C[3][5] * pow(D.xdata[i], 3)) + (C[4][5] * pow(D.xdata[i], 4));
            }
            done = TRUE; 
        }
    }




    /* Output file section: */
    done = FALSE;

    while (!done) {  
        printf("Would you like to store your data in an output file? (y/n): ");
        scanf("  %c", &choice);

        if (choice == 'Y' || choice == 'y') {
            /* Ask the user to enter the name of their output file */
            printf("Enter the name of output file with saved results data:");
            scanf("%s", outputfile);
        
            /* Attempt to open file */
            fw = fopen(outputfile, "w");
            if (fw == NULL) {
                 printf("Unable to open file\n");
                return 1;
            }
            //else printf("\nfile successfully opened...\n");

            /* Print to file and print on screen */
            fprintf(fw, "Results Output File From Project 2 - EE160\n");
            fprintf(fw, "==========================================\n");
            fprintf(fw, "Input file read:        %s\n", outputfile);
            fprintf(fw, "Original data series (x,y) and generated fit are given below\n\n");
            fprintf(fw, "      x   ,     y   ,     ycalc\n");
            fprintf(fw, "     ===  ,    ===  ,     ======\n");

            for (i = 0; i < c; i++) {
                fprintf(fw, " %8.3lf , %8.3lf , %8.3lf\n", D.xdata[i], D.ydata[i], D.ycalc[i]);
            }

            fprintf(fw, "\nPolynomial order chosen for fit: %i\n\n", k);

            while (!done) {
                if ( k == 1 ) {
                    fprintf(fw, "The linear least-squares best fit to data is: \n");
                    fprintf(fw, "y = %.5lf*x^0 + %.5lf*x^1\n\n", C[0][2], C[1][2]);
                    done = TRUE; 
                }
                if ( k == 2 ) {
                    fprintf(fw, "The linear least-squares best fit to data is: \n");
                    fprintf(fw, "y = %.5lf*x^0 + %.5lf*x^1 + %.5lf*x^2\n\n", C[0][3], C[1][3], C[2][3]);
                    done = TRUE; 
                }
                if ( k == 3 ) {
                    fprintf(fw, "The linear least-squares best fit to data is: \n");
                    fprintf(fw, "y = %.5lf*x^0 + %.5lf*x^1 + %.5lf*x^2 + %.5lf*x^3\n\n", C[0][4], C[1][4], C[2][4], C[3][4]);
                    done = TRUE;
                }
                if ( k == 4 ) {
                    fprintf(fw, "The linear least-squares best fit to data is: \n");
                    fprintf(fw, "y = %.5lf*x^0 + %.5lf*x^1 + %.5lf*x^2 + %.5lf*x^3 + %.5lf*x^4\n\n", C[0][5], C[1][5], C[2][5], C[3][5], C[4][5]);
                    done = TRUE;
                }
            }

            fprintf(fw, "End of output file.");

            /* Close file and exit */
            fclose(fw);

            printf("Your results are written to '%s'.\n", outputfile);
            done = TRUE;
        }
        else if (choice == 'N' || choice =='n') {
            printf("You chose to have no data written to an output file");
            done = TRUE;
        }
        else {
            printf("*** Error, please enter 'y' or 'n': ");
            scanf(" %c", &choice);    // Retry 
        }
    }
        
    return 0;
    
}