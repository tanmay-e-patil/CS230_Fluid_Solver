

#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string.h>


using namespace std;

/* This is the class representing fluid quantities such as density and velocity
 * on the MAC grid. It saves attributes such as offset from the top left grid
 * cell, grid width and height as well as cell size.
 * 
 * It also contains two memory buffers: A source (src) buffer and a
 * destination (dst) buffer.
 * Most operations on fluid quantities can be done in-place; that is, they
 * write to the same buffer they're reading from (which is always src).
 * However, some operations, such as advection, cannot be done in-place.
 * Instead, they will write to the dst buffer. Once the operation is
 * completed, flip() can be called to swap the source and destination buffers,
 * such that the result of the operation is visible to subsequent operations.
 */
class FluidQuantity {
    /* Memory buffers for fluid quantity */
    double *src;
    double *dst;

    /* Width and height */
    int width;
    int height;
    /* X and Y offset from top left grid cell.
     * This is (0.5,0.5) for centered quantities such as density,
     * and (0.0, 0.5) or (0.5, 0.0) for quantities like the velocity.
     */
    double x_offset;
    double y_offset;
    /* Grid cell size */
    double cell_size;
    
public:
    FluidQuantity(int w, int h, double xo, double yo, double hx)
            : width(w), height(h), x_offset(xo), y_offset(yo), cell_size(hx) {
        src = new double[width*height];
        dst = new double[width*height];
                
        memset(src, 0, width*height*sizeof(double));
    }
    
    ~FluidQuantity() {
        delete[] src;
        delete[] dst;
    }
    
    void flip() {
        swap(src, dst);
    }
    
    
    /* Read-only and read-write access to grid cells */
    double at(int x, int y) const {
        return src[x + y*width];
    }
    
    double &at(int x, int y) {
        return src[x + y*width];
    }
    
   
    
    /* Advect grid in velocity field u, v with given timestep */
    void advect(double timestep, const FluidQuantity &u, const FluidQuantity &v) {
       
    }
    
    /* Sets fluid quantity inside the given rect to value `v' */
    void addInflow(double x0, double y0, double x1, double y1, double v) {
        int ix0 = (int)(x0/cell_size - x_offset);
        int iy0 = (int)(y0/cell_size - y_offset);
        int ix1 = (int)(x1/cell_size - x_offset);
        int iy1 = (int)(y1/cell_size - y_offset);
        
        for (int y = max(iy0, 0); y < min(iy1, height); y++)
            for (int x = max(ix0, 0); x < min(ix1, height); x++)
                if (fabs(src[x + y*width]) < fabs(v))
                    src[x + y*width] = v;
    }
};