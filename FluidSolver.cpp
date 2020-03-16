

#include"FluidQuantity.h"

/* Fluid solver class. Sets up the fluid quantities, forces incompressibility
 * performs advection and adds inflows.
 */
class FluidSolver {
    /* Fluid quantities */
    FluidQuantity *d;
    FluidQuantity *ux;
    FluidQuantity *uy;
    
    /* Width and height */
    int width;
    int height;
    
    /* Grid cell size and fluid density */
    double cell_size;
    double fluid_density;
    
    /* Arrays for: */
    double *r; /* Right hand side of pressure solve */
    double *p; /* Pressure solution */
    
    
    /* Builds the pressure right hand side as the negative divergence */
    void buildRHS() {
        double scale = 1.0/cell_size;
        
        for (int y = 0, index = 0; y < height; y++) {
            for (int x = 0; x < width; x++, index++) {
                r[index] = -scale*(ux->at(x + 1, y) - ux->at(x, y) +
                                  uy->at(x, y + 1) - uy->at(x, y));
            }
        }
    }
    
    /* Performs the pressure solve using Gauss-Seidel.
     * The solver will run as long as it takes to get the relative error below
     * a threshold, but will never exceed `limit' iterations
     */
    void project(int limit, double timestep) {
        double scale = timestep/(fluid_density*cell_size*cell_size);
        
        double maxDelta;
        for (int iter = 0; iter < limit; iter++) {
            maxDelta = 0.0;
            for (int y = 0, index = 0; y < height; y++) {
                for (int x = 0; x < width; x++, index++) {
                    int index = x + y*width;
                    
                    double diag = 0.0, offDiag = 0.0;
                    
                    /* Here we build the matrix implicitly as the five-point
                     * stencil. Grid borders are assumed to be solid, i.e.
                     * there is no fluid outside the simulation domain.
                     */
                    if (x > 0) {
                        diag    += scale;
                        offDiag -= scale*p[index - 1];
                    }
                    if (y > 0) {
                        diag    += scale;
                        offDiag -= scale*p[index - width];
                    }
                    if (x < width - 1) {
                        diag    += scale;
                        offDiag -= scale*p[index + 1];
                    }
                    if (y < height - 1) {
                        diag    += scale;
                        offDiag -= scale*p[index + width];
                    }

                    double newP = (r[index] - offDiag)/diag;
                    
                    maxDelta = max(maxDelta, fabs(p[index] - newP));
                    
                    p[index] = newP;
                }
            }

            if (maxDelta < 1e-5) {
                printf("Exiting solver after %d iterations, maximum error is %f\n", iter, maxDelta);
                return;
            }
        }
        
        printf("Exceeded budget of %d iterations, maximum error was %f\n", limit, maxDelta);
    }
    
    /* Applies the computed pressure to the velocity field */
    void applyPressure(double timestep) {
        double scale = timestep/(fluid_density*cell_size);
        
        for (int y = 0, index = 0; y < height; y++) {
            for (int x = 0; x < width; x++, index++) {
                ux->at(x,     y    ) -= scale*p[index];
                ux->at(x + 1, y    ) += scale*p[index];
                uy->at(x,     y    ) -= scale*p[index];
                uy->at(x,     y + 1) += scale*p[index];
            }
        }
        
        for (int y = 0; y < height; y++)
            ux->at(0, y) = ux->at(width, y) = 0.0;
        for (int x = 0; x < width; x++)
            uy->at(x, 0) = uy->at(x, height) = 0.0;
    }
    
public:
    FluidSolver(int w, int h, double density) : width(w), height(h), fluid_density(density) {
        cell_size = 1.0/min(w, h);
        
        d = new FluidQuantity(width,     height,     0.5, 0.5, cell_size);
        ux = new FluidQuantity(width + 1, height,     0.0, 0.5, cell_size);
        uy = new FluidQuantity(width,     height + 1, 0.5, 0.0, cell_size);
        
        r = new double[width*height];
        p = new double[width*height];
        
        memset(p, 0, width*height*sizeof(double));
    }
    
    ~FluidSolver() {
        delete d;
        delete ux;
        delete uy;
        
        delete[] r;
        delete[] p;
    }
    
    
    
    /* Set density and x/y velocity in given rectangle to d/ux/uy, respectively */
    void addInflow(double x, double y, double w, double h, double density, double u, double v) {
        d->addInflow(x, y, x + w, y + h, density);
        ux->addInflow(x, y, x + w, y + h, u);
        uy->addInflow(x, y, x + w, y + h, v);
    }
};