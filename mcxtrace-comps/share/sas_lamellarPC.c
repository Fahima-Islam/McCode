// GENERATED CODE --- DO NOT EDIT ---
// Code is produced by sasmodels.gen from sasmodels/models/MODEL.c

#ifdef __OPENCL_VERSION__
# define USE_OPENCL
#endif

#define USE_KAHAN_SUMMATION 0

// If opencl is not available, then we are compiling a C function
// Note: if using a C++ compiler, then define kernel as extern "C"
#ifndef USE_OPENCL
#  ifdef __cplusplus
     #include <cstdio>
     #include <cmath>
     using namespace std;
     #if defined(_MSC_VER)
     #   define kernel extern "C" __declspec( dllexport )
         inline float trunc(float x) { return x>=0?floor(x):-floor(-x); }
	 inline float fmin(float x, float y) { return x>y ? y : x; }
	 inline float fmax(float x, float y) { return x<y ? y : x; }
     #else
     #   define kernel extern "C"
     #endif
     inline void SINCOS(float angle, float &svar, float &cvar) { svar=sin(angle); cvar=cos(angle); }
#  else
     #include <stdio.h>
     #include <tgmath.h> // C99 type-generic math, so sin(float) => sinf
     // MSVC doesn't support C99, so no need for dllexport on C99 branch
     #define kernel
     #define SINCOS(angle,svar,cvar) do {const float _t_=angle; svar=sin(_t_);cvar=cos(_t_);} while (0)
#  endif
#  define global
#  define local
#  define constant const
// OpenCL powr(a,b) = C99 pow(a,b), b >= 0
// OpenCL pown(a,b) = C99 pow(a,b), b integer
#  define powr(a,b) pow(a,b)
#  define pown(a,b) pow(a,b)
#else
#  ifdef USE_SINCOS
#    define SINCOS(angle,svar,cvar) svar=sincos(angle,&cvar)
#  else
#    define SINCOS(angle,svar,cvar) do {const float _t_=angle; svar=sin(_t_);cvar=cos(_t_);} while (0)
#  endif
#endif

// Standard mathematical constants:
//   M_E, M_LOG2E, M_LOG10E, M_LN2, M_LN10, M_PI, M_PI_2=pi/2, M_PI_4=pi/4,
//   M_1_PI=1/pi, M_2_PI=2/pi, M_2_SQRTPI=2/sqrt(pi), SQRT2, SQRT1_2=sqrt(1/2)
// OpenCL defines M_constant_F for float constants, and nothing if float
// is not enabled on the card, which is why these constants may be missing
#ifndef M_PI
#  define M_PI 3.141592653589793f
#endif
#ifndef M_PI_2
#  define M_PI_2 1.570796326794897f
#endif
#ifndef M_PI_4
#  define M_PI_4 0.7853981633974483f
#endif

// Non-standard pi/180, used for converting between degrees and radians
#ifndef M_PI_180
#  define M_PI_180 0.017453292519943295f
#endif


#define VOLUME_PARAMETERS thickness
#define VOLUME_WEIGHT_PRODUCT thickness_w
#define VOLUME_PARAMETER_DECLARATIONS float thickness
#define IQ_KERNEL_NAME lamellarPC_Iq
#define IQ_PARAMETERS thickness, Nlayers, spacing, spacing_polydisp, sld, solvent_sld
#define IQ_FIXED_PARAMETER_DECLARATIONS const float scale, \
    const float background, \
    const float Nlayers, \
    const float spacing, \
    const float spacing_polydisp, \
    const float sld, \
    const float solvent_sld
#define IQ_WEIGHT_PRODUCT thickness_w
#define IQ_DISPERSION_LENGTH_DECLARATIONS const int Nthickness
#define IQ_DISPERSION_LENGTH_SUM Nthickness
#define IQ_OPEN_LOOPS     for (int thickness_i=0; thickness_i < Nthickness; thickness_i++) { \
      const float thickness = loops[2*(thickness_i)]; \
      const float thickness_w = loops[2*(thickness_i)+1];
#define IQ_CLOSE_LOOPS     }
#define IQXY_KERNEL_NAME lamellarPC_Iqxy
#define IQXY_PARAMETERS thickness, Nlayers, spacing, spacing_polydisp, sld, solvent_sld
#define IQXY_FIXED_PARAMETER_DECLARATIONS const float scale, \
    const float background, \
    const float Nlayers, \
    const float spacing, \
    const float spacing_polydisp, \
    const float sld, \
    const float solvent_sld
#define IQXY_WEIGHT_PRODUCT thickness_w
#define IQXY_DISPERSION_LENGTH_DECLARATIONS const int Nthickness
#define IQXY_DISPERSION_LENGTH_SUM Nthickness
#define IQXY_OPEN_LOOPS     for (int thickness_i=0; thickness_i < Nthickness; thickness_i++) { \
      const float thickness = loops[2*(thickness_i)]; \
      const float thickness_w = loops[2*(thickness_i)+1];
#define IQXY_CLOSE_LOOPS     }
#define IQXY_PARAMETER_DECLARATIONS float thickness, float Nlayers, float spacing, float spacing_polydisp, float sld, float solvent_sld

/*	Lamellar_ParaCrystal - Pedersen's model

*/
float Iq(float qval,
      float th,
      float Nlayers, 
	  float davg, 
	  float pd,
      float sld,
      float solvent_sld);
float paraCryst_sn(float ww, float qval, float davg, long Nlayers, float an);
float paraCryst_an(float ww, float qval, float davg, long Nlayers);

float Iq(float qval,
      float th,
      float Nlayers, 
	  float davg, 
	  float pd,
      float sld,
      float solvent_sld)
{
    
	float inten,contr,xn;
	float xi,ww,Pbil,Znq,Snq,an;
	long n1,n2;
	
	contr = sld - solvent_sld;
	//get the fractional part of Nlayers, to determine the "mixing" of N's
	
	n1 = (long)trunc(Nlayers);		//rounds towards zero
	n2 = n1 + 1;
	xn = (float)n2 - Nlayers;			//fractional contribution of n1
	
	ww = exp(-qval*qval*pd*pd*davg*davg/2.0f);

	//calculate the n1 contribution
	an = paraCryst_an(ww,qval,davg,n1);
	Snq = paraCryst_sn(ww,qval,davg,n1,an);
	
	Znq = xn*Snq;
	
	//calculate the n2 contribution
	an = paraCryst_an(ww,qval,davg,n2);
	Snq = paraCryst_sn(ww,qval,davg,n2,an);

	Znq += (1.0f-xn)*Snq;
	
	//and the independent contribution
	Znq += (1.0f-ww*ww)/(1.0f+ww*ww-2.0f*ww*cos(qval*davg));
	
	//the limit when Nlayers approaches infinity
//	Zq = (1-ww^2)/(1+ww^2-2*ww*cos(qval*davg))
	
	xi = th/2.0f;		//use 1/2 the bilayer thickness
	Pbil = (sin(qval*xi)/(qval*xi))*(sin(qval*xi)/(qval*xi));
	
	inten = 2.0f*M_PI*contr*contr*Pbil*Znq/(qval*qval);
	inten *= 1.0e-04f;
//printf("q=%.7fe wwm1=%g ww=%.5fe an=% 12.5fe Snq=% 12.5fe Znq=% 12.5fe Pbil=% 12.5fe\n",qval,wwm1,ww,an,Snq,Znq,Pbil);
	return(inten);
}

// functions for the lamellar paracrystal model
float
paraCryst_sn(float ww, float qval, float davg, long Nlayers, float an) {
	
	float Snq;

	Snq = an/( (float)Nlayers*pow((1.0f+ww*ww-2.0f*ww*cos(qval*davg)),2) );
	
	return(Snq);
}

float
paraCryst_an(float ww, float qval, float davg, long Nlayers) {
	
	float an;
	
	an = 4.0f*ww*ww - 2.0f*(ww*ww*ww+ww)*cos(qval*davg);
	an -= 4.0f*pow(ww,(Nlayers+2))*cos((float)Nlayers*qval*davg);
	an += 2.0f*pow(ww,(Nlayers+3))*cos((float)(Nlayers-1)*qval*davg);
	an += 2.0f*pow(ww,(Nlayers+1))*cos((float)(Nlayers+1)*qval*davg);
	
	return(an);
}



float form_volume(VOLUME_PARAMETER_DECLARATIONS);
float form_volume(VOLUME_PARAMETER_DECLARATIONS) {
    
    return 1.0f;
    
}


float Iqxy(float qx, float qy, IQXY_PARAMETER_DECLARATIONS);
float Iqxy(float qx, float qy, IQXY_PARAMETER_DECLARATIONS) {
    
    return Iq(sqrt(qx*qx+qy*qy), IQ_PARAMETERS);
    
}


/*
    ##########################################################
    #                                                        #
    #   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   #
    #   !!                                              !!   #
    #   !!  KEEP THIS CODE CONSISTENT WITH KERNELPY.PY  !!   #
    #   !!                                              !!   #
    #   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   #
    #                                                        #
    ##########################################################
*/

#ifdef IQ_KERNEL_NAME
kernel void IQ_KERNEL_NAME(
    global const float *q,
    global float *result,
    const int Nq,
#ifdef IQ_OPEN_LOOPS
  #ifdef USE_OPENCL
    global float *loops_g,
  #endif
    local float *loops,
    const float cutoff,
    IQ_DISPERSION_LENGTH_DECLARATIONS,
#endif
    IQ_FIXED_PARAMETER_DECLARATIONS
    )
{
#ifdef USE_OPENCL
  #ifdef IQ_OPEN_LOOPS
  // copy loops info to local memory
  event_t e = async_work_group_copy(loops, loops_g, (IQ_DISPERSION_LENGTH_SUM)*2, 0);
  wait_group_events(1, &e);
  #endif

  int i = get_global_id(0);
  if (i < Nq)
#else
  #pragma omp parallel for
  for (int i=0; i < Nq; i++)
#endif
  {
    const float qi = q[i];
#ifdef IQ_OPEN_LOOPS
    float ret=0.0f, norm=0.0f;
  #ifdef VOLUME_PARAMETERS
    float vol=0.0f, norm_vol=0.0f;
  #endif
    IQ_OPEN_LOOPS
    //for (int radius_i=0; radius_i < Nradius; radius_i++) {
    //  const float radius = loops[2*(radius_i)];
    //  const float radius_w = loops[2*(radius_i)+1];

    const float weight = IQ_WEIGHT_PRODUCT;
    if (weight > cutoff) {
      const float scattering = Iq(qi, IQ_PARAMETERS);
      // allow kernels to exclude invalid regions by returning NaN
      if (!isnan(scattering)) {
        ret += weight*scattering;
        norm += weight;
      #ifdef VOLUME_PARAMETERS
        const float vol_weight = VOLUME_WEIGHT_PRODUCT;
        vol += vol_weight*form_volume(VOLUME_PARAMETERS);
        norm_vol += vol_weight;
      #endif
      }
    //else { printf("exclude qx,qy,I:%g,%g,%g\n",qi,scattering); }
    }
    IQ_CLOSE_LOOPS
  #ifdef VOLUME_PARAMETERS
    if (vol*norm_vol != 0.0f) {
      ret *= norm_vol/vol;
    }
  #endif
    result[i] = scale*ret/norm+background;
#else
    result[i] = scale*Iq(qi, IQ_PARAMETERS) + background;
#endif
  }
}
#endif


#ifdef IQXY_KERNEL_NAME
kernel void IQXY_KERNEL_NAME(
    global const float *qx,
    global const float *qy,
    global float *result,
    const int Nq,
#ifdef IQXY_OPEN_LOOPS
  #ifdef USE_OPENCL
    global float *loops_g,
  #endif
    local float *loops,
    const float cutoff,
    IQXY_DISPERSION_LENGTH_DECLARATIONS,
#endif
    IQXY_FIXED_PARAMETER_DECLARATIONS
    )
{
#ifdef USE_OPENCL
  #ifdef IQXY_OPEN_LOOPS
  // copy loops info to local memory
  event_t e = async_work_group_copy(loops, loops_g, (IQXY_DISPERSION_LENGTH_SUM)*2, 0);
  wait_group_events(1, &e);
  #endif

  int i = get_global_id(0);
  if (i < Nq)
#else
  #pragma omp parallel for
  for (int i=0; i < Nq; i++)
#endif
  {
    const float qxi = qx[i];
    const float qyi = qy[i];
    #if USE_KAHAN_SUMMATION
    float accumulated_error = 0.0f;
    #endif
#ifdef IQXY_OPEN_LOOPS
    float ret=0.0f, norm=0.0f;
    #ifdef VOLUME_PARAMETERS
    float vol=0.0f, norm_vol=0.0f;
    #endif
    IQXY_OPEN_LOOPS
    //for (int radius_i=0; radius_i < Nradius; radius_i++) {
    //  const float radius = loops[2*(radius_i)];
    //  const float radius_w = loops[2*(radius_i)+1];

    const float weight = IQXY_WEIGHT_PRODUCT;
    if (weight > cutoff) {

      const float scattering = Iqxy(qxi, qyi, IQXY_PARAMETERS);
      if (!isnan(scattering)) { // if scattering is bad, exclude it from sum
      //if (scattering >= 0.0f) { // scattering cannot be negative
        // TODO: use correct angle for spherical correction
        // Definition of theta and phi are probably reversed relative to the
        // equation which gave rise to this correction, leading to an
        // attenuation of the pattern as theta moves through pi/2.f  Either
        // reverse the meanings of phi and theta in the forms, or use phi
        // rather than theta in this correction.  Current code uses cos(theta)
        // so that values match those of sasview.
      #if defined(IQXY_HAS_THETA) // && 0
        const float spherical_correction
          = (Ntheta>1 ? fabs(cos(M_PI_180*theta))*M_PI_2:1.0f);
        const float next = spherical_correction * weight * scattering;
      #else
        const float next = weight * scattering;
      #endif
      #if USE_KAHAN_SUMMATION
        const float y = next - accumulated_error;
        const float t = ret + y;
        accumulated_error = (t - ret) - y;
        ret = t;
      #else
        ret += next;
      #endif
        norm += weight;
      #ifdef VOLUME_PARAMETERS
        const float vol_weight = VOLUME_WEIGHT_PRODUCT;
        vol += vol_weight*form_volume(VOLUME_PARAMETERS);
      #endif
        norm_vol += vol_weight;
      }
      //else { printf("exclude qx,qy,I:%g,%g,%g\n",qi,scattering); }
    }
    IQXY_CLOSE_LOOPS
  #ifdef VOLUME_PARAMETERS
    if (vol*norm_vol != 0.0f) {
      ret *= norm_vol/vol;
    }
  #endif
    result[i] = scale*ret/norm+background;
#else
    result[i] = scale*Iqxy(qxi, qyi, IQXY_PARAMETERS) + background;
#endif
  }
}
#endif
