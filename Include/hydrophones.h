/**
 * @file
 * 
 * @brief Small library that implements the 
 * hydrophones for the AUV for Vortex NTNU
 * 
 * @note Could be improved by using the pose
 * from ROS. Should be implemented in the 
 * future
 * 
 * @note It is thought that the hydrophones are
 * placed in a form of a triangle. Therefore, we
 * have one on front starboard, one on front port
 * and one on the stern in the middle. Could be changed
 * in the future
 * 
 *                    BOW
 *                      
 *      Port                       Starboard
 *                      
 *       HYD ''''''''''''''''''''' HYD
 *           '''''''''''''''''''''
 *           ''''''''' A '''''''''
 *           ''''''''' U '''''''''
 *           ''''''''' V '''''''''
 *           '''''''''''''''''''''
 *           '''''''''''''''''''''
 *           '''''''''HYD'''''''''
 * 
 *                   STERN
 * 
 * @note The position of the hydrophones are relative
 * to the center of the AUV
 */
#ifndef ACOUSTICS_HYDROPHONES_H
#define ACOUSTICS_HYDROPHONES_H

#include "trilateration.h"


/**
 * @brief Defines that indicate the setup of the hardware on the AUV
 * 
 * These variables must be changed according to the hydrophones used.
 * As of 03.01.21, three hydrophones of the type Benthowave BII-7014FG 
 * is used. See the datasheet at 
 * https://www.benthowave.com/products/Specs/BII-7014FGPGDatasheet.pdf
 * for more information
 * 
 * @note Position of the hydrophones are relative to the center of the 
 * AUV
 */
#ifndef HYDROPHONE_DETAILS
#define HYDROPHONE_DETAILS

  #define NUM_HYDROPHONES   3         /* Number of hydrophones used on the AUV        */
  #define HYD_PREAMP_DB     40        /* Number of dB the signal is preamplifies      */
  #define HYD_FFVS          -173      /* Average FFVS for 20 - 40 kHz [dB V/μPa]      */  

  #define PORT_HYD_X        -1.0      /* x - position of port hydrophone      [m]     */
  #define PORT_HYD_Y        0.0       /* y - position of port hydrophone      [m]     */
  #define PORT_HYD_Z        0.0       /* z - position of port hydrophone      [m]     */

  #define STARBOARD_HYD_X   1.0       /* x - position of starboard hydrophone [m]     */
  #define STARBOARD_HYD_Y   0.0       /* y - position of starboard hydrophone [m]     */
  #define STARBOARD_HYD_Z   0.0       /* z - position of starboard hydrophone [m]     */

  #define STERN_HYD_X       0.0       /* x - position of stern hydrophone     [m]     */
  #define STERN_HYD_Y       -1.0      /* y - position of stern hydrophone     [m]     */
  #define STERN_HYD_Z       0.0       /* z - position of stern hydrophone     [m]     */

#endif /* HYDROPHONE_DETAILS */


namespace HYDROPHONES{

/**
 * @brief Globals that describes the
 * hydrophones position compared to the
 * center of the AUV. Must be updated in the future
 * when the placement of the hydrophones are known
 */
TRILATERATION::Pos pos_hyd_port{PORT_HYD_X, PORT_HYD_Y, PORT_HYD_Z};
TRILATERATION::Pos pos_hyd_starboard{STARBOARD_HYD_X, STARBOARD_HYD_Y, STARBOARD_HYD_Z};
TRILATERATION::Pos pos_hyd_stern{STERN_HYD_X, STERN_HYD_Y, STERN_HYD_Z};


/**
 * @brief Class for the hydrophones. 
 * Combines the basic functions for
 * each hydrophone. Makes it a lot 
 * easier to implement
 * 
 * @warning Must be evaluated if this
 * class is really necessary.
 */
class Hydrophones{
private:
    /**
    * @brief Position of each hydrophone 
    */
    TRILATERATION::Pos pos;

    /**
    * @brief The lag calculated for the last sample
    */
    uint32_t last_lag;

    /**
     * @brief The intensity calculated for the last sample
     */
    float32_t last_intensity;

    /**
    * @brief The dataset for one hydrophone 
    */
    float32_t* p_data;

    /**
     * @brief The autocorrelated data-sequence
     * 
     * @warning The length of the autocorrelation will be
     * 2 * length(p_data) - 1
     */
    float32_t* p_autocorr_data;
     
public:
    /**
    * @brief Constructor and destructor 
    */
    Hydrophones(TRILATERATION::Pos pos);
    ~Hydrophones();
    
    /**
    * @brief Function to return the last calculated lag 
    */
    uint32_t get_lag() const { return last_lag; } 

    /**
     * @brief Function to return the intensity
     */
    float32_t get_intensity() const { return last_intensity; }

    /**
    * @brief Function to analyze the data
    * 
    * The raw data is filtered using a second-order biquad DF1 
    * IIR filter to eliminate unwanted frequencies. Thereafter
    * the autocorrelation is found and used to calculate the lag, 
    * intensity and distance are thereafter estimated 
    * from the filtered data
    */
    void analyze_data(float32_t* p_raw_data);
}; /* class Hydrophones */

} /* namespace HYDROPHONES */

#endif // ACOUSTICS_HYDROPHONES_H



