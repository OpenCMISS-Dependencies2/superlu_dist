#pragma once 
// #include "lupanels.hpp"
#include "xgstrf2.hpp"
template <typename Ftype>
xlpanel_t<Ftype>::xlpanel_t(int_t k, int_t *lsub, Ftype* lval, int_t *xsup, int_t isDiagIncluded)
{
    // set the value
    val = lval;
    int_t nlb = lsub[0];
    int_t nzrow = lsub[1];
    int_t lIndexSize = LPANEL_HEADER_SIZE + 2 * nlb + 1 + nzrow;
    //
    index = (int_t *)SUPERLU_MALLOC(sizeof(int_t) * lIndexSize);
    index[0] = nlb;
    index[1] = nzrow;
    index[2] = isDiagIncluded; //either one or zero
    index[3] = SuperSize(k);
    index[LPANEL_HEADER_SIZE + nlb] = 0; // starting of prefix sum is zero
    // now start the loop
    int_t blkIdPtr = LPANEL_HEADER_SIZE;
    int_t pxSumPtr = LPANEL_HEADER_SIZE + nlb + 1;
    int_t rowIdxPtr = LPANEL_HEADER_SIZE + 2 * nlb + 1;
    int_t lsub_ptr = BC_HEADER;
    for (int_t lb = 0; lb < nlb; lb++)
    {
        /**
        *   BLOCK DESCRIPTOR (of size LB_DESCRIPTOR)  |
        *       block number (global)              |
        *       number of full rows in the block 
        ***/
        int_t global_id = lsub[lsub_ptr];
        int_t nrows = lsub[lsub_ptr + 1];

        index[blkIdPtr++] = global_id;
        index[pxSumPtr] = nrows + index[pxSumPtr - 1];
        pxSumPtr++;

        int_t firstRow = xsup[global_id];
        for (int rowId = 0; rowId < nrows; rowId++)
        {
            //only storing relative distance
            index[rowIdxPtr++] = lsub[lsub_ptr + LB_DESCRIPTOR + rowId] - firstRow;
        }
        // Update the lsub_ptr
        lsub_ptr += LB_DESCRIPTOR + nrows;
    }
    return;
}

//TODO: can be optimized
template <typename Ftype>
int_t xlpanel_t<Ftype>::find(int_t k)
{
    for (int_t i = 0; i < nblocks(); i++)
    {
        if (k == gid(i))
            return i;
    }
    //TODO: it shouldn't come here
    return GLOBAL_BLOCK_NOT_FOUND;
}

template <typename Ftype>
int_t xlpanel_t<Ftype>::panelSolve(int_t ksupsz, Ftype* DiagBlk, int_t LDD)
{
    if (isEmpty())
        return 0;
    Ftype* lPanelStPtr = blkPtr(0);
    int_t len = nzrows();
    if (haveDiag())
    {
        /* code */
        lPanelStPtr = blkPtr(1);
        len -= nbrow(0);
    }
    Ftype alpha = one<Ftype>(); // {1.0, 0.0}; std::complex<double> alpha = {1.0, 0.0};
    superlu_trsm<Ftype>("R", "U", "N", "N",
                  len, ksupsz, alpha, DiagBlk, LDD,
                  lPanelStPtr, LDA());
    return 0;
}



template <typename Ftype>
int_t xlpanel_t<Ftype>::diagFactor(int_t k, Ftype* UBlk, int_t LDU, 
threshPivValType<Ftype> thresh, int_t *xsup,
                           superlu_dist_options_t *options,
                           SuperLUStat_t *stat, int *info)
{
    // dgstrf2(k, val, LDA(), UBlk, LDU,
    //         thresh, xsup, options, stat, info);

    xgstrf2<Ftype>(k, val, LDA(), UBlk, LDU,
            thresh, xsup, options, stat, info);

    return 0;
}

template <typename Ftype>
int_t xlpanel_t<Ftype>::packDiagBlock(Ftype* DiagLBlk, int_t LDD)
{
    assert(haveDiag());
    assert(LDD >= nbrow(0));
    int_t nsupc = nbrow(0);
    for (int j = 0; j < nsupc; ++j)
    {
        memcpy(&DiagLBlk[j * LDD], &val[j * LDA()], nsupc * sizeof(Ftype));
    }
    return 0;
}

template <typename Ftype>
int xlpanel_t<Ftype>::getEndBlock(int iSt, int maxRows)
{
    int nlb = nblocks();
    if(iSt >= nlb )
        return nlb; 
    int iEnd = iSt; 
    int ii = iSt +1;

    while (
        stRow(ii) - stRow(iSt) <= maxRows &&
        ii < nlb)
        ii++;

#if 1
    if (stRow(ii) - stRow(iSt) > maxRows)
        iEnd = ii-1;
    else 
        iEnd =ii; 
#else 
    if (ii == nlb)
    {
        if (stRow(ii) - stRow(iSt) <= maxRows)
            iEnd = nlb;
        else
            iEnd = nlb - 1;
    }
    else
        iEnd = ii - 1;
#endif 
    return iEnd; 
}
