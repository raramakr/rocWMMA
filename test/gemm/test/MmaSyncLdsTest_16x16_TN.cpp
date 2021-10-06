
#include "MmaSyncLdsTest.h"

// Test params for 16 x 16 TN kernels
struct TestParams16x16TN : public CommonTestParams
{
    using ABLayouts = std::tuple<wmma::row_major, wmma::col_major>;
    using Base      = CommonTestParams;

    // Set up the testing context:
    // Kernel: MmaSync
    // Types: ALL + double
    // Block Sizes: 16 x 16 x BlockK
    // Layouts: TN
    using Types =
        typename Concat<typename Base::TestTypesIOC, typename Base::TestTypeDouble>::Result;
    using BlockSizes = typename Base::TestBlockSizes16x16;
    using Layouts    = typename CombineOne<ABLayouts, typename Base::TestLayoutTypes>::Result;

    // Assemble the kernel generator
    using TestParams =
        typename CombineMany<Types, typename CombineMany<BlockSizes, Layouts>::Result>::Result;
    using GeneratorImpl   = MmaSyncLdsGenerator;
    using KernelGenerator = KernelGenerator<TestParams, GeneratorImpl>;

    static inline typename KernelGenerator::ResultT kernels()
    {
        return KernelGenerator::generate();
    }
};

// Test suite for unique parameterization
class MmaSyncLdsTest16x16TN : public MmaSyncLdsTest
{
};

TEST_P(MmaSyncLdsTest16x16TN, RunKernel)
{
    this->RunKernel();
}

INSTANTIATE_TEST_SUITE_P(GemmKernelTests,
                         MmaSyncLdsTest16x16TN,
                         ::testing::Combine(::testing::ValuesIn(TestParams16x16TN::kernels()),
                                            ::testing::ValuesIn(TestParams16x16TN::threadBlocks()),
                                            ::testing::ValuesIn(TestParams16x16TN::problemSizes()),
                                            ::testing::ValuesIn(TestParams16x16TN::alphas()),
                                            ::testing::ValuesIn(TestParams16x16TN::betas())));
