#include <CL/sycl.hpp>
#include <iostream>




/*********************************************************************************/
/*********************************************************************************/
/************                                                         ************/
/********                                                                 ********/
/****                               GPU Selector                              ****/
/********                                                                 ********/
/************                                                         ************/
/*********************************************************************************/
/*********************************************************************************/
//  https://www.codeproject.com/Articles/5284853/Use-oneAPI-to-make-your-Cplusplus-application-GPU
// Use this device selector to select the Gen9 Intel GPU
class gen9_gpu_selector : public cl::sycl::device_selector
{
public:
    gen9_gpu_selector()
      : cl::sycl::device_selector()
    {}

    int operator()(cl::sycl::device const& device) const override
    {
        if (device.get_info<cl::sycl::info::device::name>()
            == "Intel(R) Gen9 HD Graphics NEO")
        {
            return 100;
        }
        return 0;
    }
};




/*********************************************************************************/
/*********************************************************************************/
/************                                                         ************/
/********                                                                 ********/
/****                        Pack_String_Into_Numeric()                       ****/
/****                                                                         ****/
/****                  32 char  packed into  8  32 bit uint                   ****/
/****                               8 x 4 = 32                                ****/
/********                                                                 ********/
/************                                                         ************/
/*********************************************************************************/
/*********************************************************************************/
inline void Pack_String_Into_Octo_Numeric( const char Input_Char_Array[32], uint Int_Block[] ) 
{
        for ( int char_index = 0; char_index < 32; char_index++ ) {
                uint current_unsigned_integer = Input_Char_Array[char_index];
                int current_block = char_index / 4;
                int shift_bits_within_block = (char_index % 4) * 8;
                uint current_unsigned_pattern = current_unsigned_integer << shift_bits_within_block;
                Int_Block[current_block] = Int_Block[current_block] | current_unsigned_pattern;
                std::cout << "Char[" << char_index << "] Block[" << current_block << "] Shift[" << shift_bits_within_block << "]  char[" << Input_Char_Array[char_index] << "] UINT[" << current_unsigned_integer << "] Packed[" << Int_Block[current_block] << "] \n";
        }  //  end: char Loop
}    //  end:  Pack_String_Into_Octo_Numeric()








/*********************************************************************************/
/*********************************************************************************/
/************                                                         ************/
/********                                                                 ********/
/****                                main()                                   ****/
/********                                                                 ********/
/************                                                         ************/
/*********************************************************************************/
/*********************************************************************************/
int main() {

    using namespace cl::sycl;

    constexpr int qnty_data_words = 56;

    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;


//  Google.com  -  209.85.231.104 
//  Microsoft.com –  207.46.170.123
//  Yahoo.com - 72.30.2.43
//  Facebook.com - 66.220.149.25
//  Wikipedia.com - 208.80.152.2
//  Dell.com - 143.166.83.38
//  Twitter.com - 128.242.245.116

    const char company_name[7][32] = { "Google.com", "Microsoft.com", "Yahoo.com", "Facebook.com",
                                        "Wikipedia.com", "Dell.com", "Twitter.com" };

    const char ip_address[7][32] = { "209.85.231.104", "207.46.170.123",  "72.30.2.43", "66.220.149.25",
                                      "208.80.152.2", "143.166.83.38", "128.242.245.116" };

    const char input_test_0[32] = {"Microsoft.com"};
    const char input_test_1[32] = {"Google.com"};


    uint company_name_array[56];    //    7 Names (threads)  *  8 uint   (4 Bytes per uint)    =    56  Words Total
    uint input_test_0_array[8] = {0};
    uint input_test_1_array[8] = {0};
    uint result_data_0_array[8] = {0};
    uint result_data_1_array[8] = {0};
    uint result_index_array[7] = {0};
    uint comparison_results_0_array[56] = {0};
    uint comparison_results_1_array[56] = {0};


        for (int pattern_index = 0; pattern_index < 7; pattern_index++)  {
            uint packed_integer[8] = {0};
            Pack_String_Into_Octo_Numeric( company_name[pattern_index], packed_integer ); 
            for ( int block_index = 0; block_index < 8; block_index++ ) {
                int current_index = (pattern_index * 8) + block_index;
                company_name_array[current_index] = packed_integer[block_index];
            }
            std::cout << "PackedCompanyName[" << pattern_index << "][" << company_name[pattern_index] << "][" << packed_integer[0] << "][" << packed_integer[1] << "][" << packed_integer[2] << "][" << packed_integer[3] << "][" << packed_integer[4] << "][" << packed_integer[5] << "][" << packed_integer[6] << "][" << packed_integer[7] << "]\n";
            std::cout<< "" << std::endl;
        }  // end:  Pattern Loop


    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;


        std::cout<< "        ********        BEGIN:  Pack Input Test 0  [" << input_test_0  << "]        ********" << std::endl;
        uint packed_integer_0[8] = {0};
        Pack_String_Into_Octo_Numeric( input_test_0, packed_integer_0 ); 

        for ( int block_index = 0; block_index < 8; block_index++ ) 
             input_test_0_array[block_index] = packed_integer_0[block_index];
        std::cout << "PackedCompanyName[" << input_test_0 << "][" << packed_integer_0[0] << "][" << packed_integer_0[1] << "][" << packed_integer_0[2] << "][" << packed_integer_0[3] << "][" << packed_integer_0[4] << "][" << packed_integer_0[5] << "][" << packed_integer_0[6] << "][" << packed_integer_0[7] << "]\n";
        std::cout<< "        ********        END:    Pack Input Test 0  [" << input_test_0  << "]        ********" << std::endl;


    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;


        std::cout<< "        ********        BEGIN:  Pack Input Test 1  [" << input_test_1  << "]        ********" << std::endl;
        uint packed_integer_1[8] = {0};
        Pack_String_Into_Octo_Numeric( input_test_1, packed_integer_1 ); 

        for ( int block_index = 0; block_index < 8; block_index++ ) 
             input_test_1_array[block_index] = packed_integer_1[block_index];
        std::cout << "PackedCompanyName[" << input_test_1 << "][" << packed_integer_1[0] << "][" << packed_integer_1[1] << "][" << packed_integer_1[2] << "][" << packed_integer_1[3] << "][" << packed_integer_1[4] << "][" << packed_integer_1[5] << "][" << packed_integer_1[6] << "][" << packed_integer_1[7] << "]\n";
        std::cout<< "        ********        END:    Pack Input Test 1  [" << input_test_1  << "]        ********" << std::endl;


    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;




    constexpr int num=1024;
    auto r = range{num};
    buffer<uint> a{r};


        // Wrap the data variable in a buffer.
        cl::sycl::buffer<cl::sycl::cl_uint> company_names_buffer{range(56)};    //    7 Names (threads)  *  8 uint   =    56
        cl::sycl::buffer<cl::sycl::cl_uint> input_buffer_0{range(8)};
        cl::sycl::buffer<cl::sycl::cl_uint> input_buffer_1{range(8)};
        cl::sycl::buffer<cl::sycl::cl_uint> result_buffer_0{range(8)};
        cl::sycl::buffer<cl::sycl::cl_uint> result_buffer_1{range(8)};
        cl::sycl::buffer<cl::sycl::cl_uint> result_index_buffer{range(7)};
        cl::sycl::buffer<cl::sycl::cl_uint> comparison_results_0_buffer{range(56)};
        cl::sycl::buffer<cl::sycl::cl_uint> comparison_results_1_buffer{range(56)};



    /*************************************************************************/
    /*************************************************************************/
    /********                                                         ********/
    /****                           SYCL Block                            ****/
    /********                                                         ********/
    /*************************************************************************/
    /*************************************************************************/
    // Include all the SYCL work in a {} block to ensure all
    // SYCL tasks are completed before exiting the block.
    { 
        // Create a queue to enqueue work to

        //  https://www.codeproject.com/Articles/5284853/Use-oneAPI-to-make-your-Cplusplus-application-GPU
        //  Here, we make a custom selector to prioritize the Gen9 integrated GPU on recent Intel CPUs. 
        //  You can also choose built-in selectors such as the 
        //      cl::sycl::gpu_selector,    cl::sycl::fpga_selector,    cl::sycl::host_selector,    or just the cl::sycl::default_selector.
        cl::sycl::queue accelerator_queue{gen9_gpu_selector{}};

        // Output platform and device information.
        auto device = accelerator_queue.get_device();
        auto p_name = device.get_platform().get_info<info::platform::name>();
        std::cout << "Platform Name: " << p_name << "\n";
        auto p_version = device.get_platform().get_info<info::platform::version>();
        std::cout << "Platform Version: " << p_version << "\n";
        auto d_name = device.get_info<info::device::name>();
        std::cout << "Device Name: " << d_name << "\n";
        auto max_work_group = device.get_info<info::device::max_work_group_size>();
        std::cout << "Max Work Group: " << max_work_group << "\n";
        auto max_compute_units = device.get_info<info::device::max_compute_units>();
        std::cout << "Max Compute Units: " << max_compute_units << "\n\n";




        uint *host_array = malloc_host<uint>(qnty_data_words, accelerator_queue);
        uint *shared_array = malloc_shared<uint>(qnty_data_words, accelerator_queue);

        for (int i = 0; i < qnty_data_words; i++) {
            // Initialize hostArray on host
            // host_array[i] = i;
            host_array[i] = company_name_array[i];
        }


        uint *test_0_array = malloc_host<uint>(8, accelerator_queue);
        uint *test_1_array = malloc_host<uint>(8, accelerator_queue);

        for (int i = 0; i < 8; i++) {
            test_0_array[i] = input_test_0_array[i];
            test_1_array[i] = input_test_1_array[i];
        }




    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
		accelerator_queue.submit([&](handler& cgh) {
            accessor accessor_data{company_names_buffer, cgh};
            accessor accessor_input_0{input_buffer_0, cgh};
            accessor accessor_input_1{input_buffer_1, cgh};
            accessor accessor_result_0{result_buffer_0, cgh};
            accessor accessor_result_1{result_buffer_1, cgh};
            accessor accessor_result_index{result_index_buffer, cgh};
            accessor accessor_comparison_results_0{comparison_results_0_buffer, cgh};
            accessor accessor_comparison_results_1{comparison_results_1_buffer, cgh};
            accessor out{a, cgh};
//  		accessor out{a, h};


			cgh.parallel_for(range(7), [=](item<1> idx) {

                int loop_index = idx[0];

                if(loop_index == 0) {
                    // access sharedArray and hostArray on device
                    // shared_array[i] = host_array[i] + 1;
                    for (int word_index=0; word_index<qnty_data_words; ++word_index)  {
                        shared_array[word_index] = host_array[word_index];
                        out[word_index] =  host_array[word_index];
                    }
                }

                int current_index = 0;

                for(int i=0; i < 8; i++) {
                    current_index = (idx[0] * 8) +  i;
                    accessor_result_0[i] = test_0_array[i] - host_array[current_index];
                    accessor_comparison_results_0[current_index] = accessor_result_0[i];
                }


                for(int i=0; i < 8; i++) {
                    current_index = (idx[0] * 8) +  i;
                    accessor_result_1[i] = test_1_array[i] - host_array[current_index];
                    accessor_comparison_results_1[current_index] = accessor_result_1[i];
                }


                int combined_result_0 = 0;
                for(int i=0; i < 8; i++) 
                    combined_result_0 += accessor_result_0[i];
                if(combined_result_0 == 0) 
                    accessor_result_index[0] = idx[0];


                int combined_result_1 = 0;
                for(int i=0; i < 8; i++) 
                    combined_result_1 += accessor_result_1[i];
                if(combined_result_1 == 0) 
                    accessor_result_index[1] = idx[0];


            }); // End of the kernel function


        }); // End of the queue commands


    } // End of scope, so:wait for the queued work to complete




    /*************************************************************************/
    /*************************************************************************/
    /********                                                         ********/
    /****                           le fini                               ****/
    /********                                                         ********/
    /*************************************************************************/
    /*************************************************************************/
    host_accessor result_0{result_buffer_0};
    host_accessor result_1{result_buffer_1};
    host_accessor result_index{result_index_buffer};
    host_accessor comparisons_0{comparison_results_0_buffer};
    host_accessor comparisons_1{comparison_results_1_buffer};
    host_accessor company_names{company_names_buffer};    //    7 Names (threads)  *  8 uint   =    56
    host_accessor input_0{input_buffer_0};
    host_accessor input_1{input_buffer_1};


    host_accessor result{a};


    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;


    for (int word_index=0; word_index<qnty_data_words; ++word_index)  {
        std::cout << "Device Stored Template Word[" << word_index << "] [" << result[word_index] << "] \n";
    }


    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;


    // print result


    std::cout<< "        ********        Comparison Results:        ******** " << std::endl;

    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;

    std::cout<< "Test[0]   Input[" << input_test_0 << "] Matches Pattern[" << result_index[0] << "]   Result[" << ip_address[result_index[0]] << "] " << std::endl;

    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
 
    std::cout<< "Test[1]   Input[" << input_test_1 << "]  Matches Pattern[" << result_index[1] << "]   Result[" << ip_address[result_index[1]] << "] " << std::endl;

    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;
    std::cout<< "" << std::endl;

    return 0;

}  //  end:  main()

