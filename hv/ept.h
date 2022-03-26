#pragma once

#include <ia32.hpp>

namespace hv {

// number of PDs in the EPT paging structures
inline constexpr size_t ept_pd_count = 64;

struct vcpu_ept_data {
  // EPT PML4
  alignas(0x1000) ept_pml4e pml4[512];

  // EPT PDPT - a single one covers 512GB of physical memory
  alignas(0x1000) ept_pdpte pdpt[512];
  static_assert(ept_pd_count <= 512, "Only 512 EPT PDs are supported!");

  // an array of EPT PDs - each PD covers 1GB
  union {
    alignas(0x1000) ept_pde     pds[ept_pd_count][512];
    alignas(0x1000) ept_pde_2mb pds_2mb[ept_pd_count][512];
  };
};

// identity-map the EPT paging structures
void prepare_ept(vcpu_ept_data& ept);

// update the memory types in the EPT paging structures based on the MTRRs.
// this function should only be called from root-mode during vmx-operation.
void update_ept_memory_type(vcpu_ept_data& ept);

} // namespace hv

