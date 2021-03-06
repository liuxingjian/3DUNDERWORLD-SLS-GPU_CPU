#pragma once
#include <core/PointCloud.hpp>
#include <memory>
#include "Log.hpp"
#include "Reconstructor.h"
namespace SLS {

// CPU reconstructor
/*! Reconstruction buckets of cameras
 *
 * In the reconstruction, camera pixels are assigned to different projector
 * pixels.
 * Generally, one projector pixels contains more than one camera pixel. For each
 * camera, we assign
 * pixels to projector pixels, and call those projector pixels buckets.
 * ```
 * ProjPixels(Buckets)   Camera pixels
 * +------------------+  +---+---+---+
 * |       0          +->+   |   |   |
 * +------------------+  +-------+---+
 * |       1          +->+   |
 * +------------------+  +----
 *
 * +------------------+  +---+---+
 * |       n          +->+   |   |
 * +------------------+  +---+---+
 *
 * ```
 * The camera pixels in the same bucket of two different cameras are considered
 * correspondent pixels,
 * depth then can be extracted from those pixels.
 */
class ReconstructorCPU : public Reconstructor {
private:
    std::vector<std::vector<std::vector<size_t>>>
        buckets_;  //!< Buckets [camIdx][ProjPixelIdx][camPixelIdx]
    void initBuckets();
    void generateBuckets();

    //! Find intersection of two projector pixel
    /*! Each projector pixel contains multiple camera pixels.
     * This function takes two buckets from two cameras. Shoot a ray from each
     * pixel to find a pair of rays with minimum distance. In this function, the
     * position of the pixel is calculated by the average position and color of
     * all pairs of rays in a projector pixel.
     *
     * The retrun value is put in a std::array<glm::vec3, 2>, in which the first
     * vec3 is the position and the second is the color
     * \param bucket0 First bucket
     * \param bucket1 Another bucket
     * \return midpoint and color
     */
    std::array<glm::vec3, 2> intersectionOfBucket_(size_t firstCameraIdx,
                                                   size_t secondCameraIdx,
                                                   size_t bucketIdx);

    /*! Similar to intersectionOfBucket_(), this function finds pair of rays
     * with minimu distance to reconstruct the depth.
     *
     * This function is not used for now since the average method yield more
     * structural result.
     */
    std::array<glm::vec3, 2> intersectionOfBucketMinDist_(size_t firstCameraIdx,
                                                   size_t secondCameraIdx,
                                                   size_t bucketIdx);

public:
    /*! Create a reconstructor with given projector size
    */
    ReconstructorCPU(const size_t projX, const size_t projY) : Reconstructor()
    {
        projector_ = new Projector(projX, projY);
    }
    ~ReconstructorCPU() override;

    // Interfaces
    //! Reconstruct point cloud.
    PointCloud reconstruct() override;

    /*! Add a camera to reconstructor\
     *
     * Accepts two camera now.
     */
    void addCamera(Camera *cam) override;
};
}  // namespace SLS
