/*  SANYAM
 *  GUPTA
 *  sanyamgu
 */


#ifndef A1_HPP
#define A1_HPP

#include <vector>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <algorithm>
#include <limits>

std::vector<int> all_to_all(std::vector<short int>& seq, const std::vector<int>& scount, const std::vector<int>& sdispl, MPI_Comm comm){
    int p = scount.size();
    std::vector<int> rcount(p,0);
    std::vector<int> rdispl(p,0);

    MPI_Alltoall(&scount[0], 1, MPI_INT, &rcount[0], 1, MPI_INT, comm);
    std::partial_sum(std::begin(rcount), std::end(rcount) - 1, std::begin(rdispl)+1);

    int n = rdispl.back() + rcount.back();
    std::vector<short int> rdata(n);

    MPI_Alltoallv(&seq[0], &scount[0], &sdispl[0], MPI_INT, &rdata[0], &rcount[0], &rdispl[0], MPI_INT, comm);
    seq = std::move(rdata);
    return std::move(rdispl);
}

void multiway_merge(std::vector<short int>& seq, std::vector<int>& scount){
    std::vector<int> out(seq.size());
    for (int i=0; i<seq.size(); i++){
        int minIndex = scount[0];
        int val = 0;
        for (int comp=1; comp<scount.size(); comp++){
            if (scount[comp] < scount[minIndex]) {
                minIndex = scount[comp];
                val = comp;
            }
        }
        out[i] = seq[minIndex];
        // seq[minIndex] = 500;
        scount[val] +=1;
    }
    for (int i=0; i<seq.size(); i++){
        seq[i] = out[i];
    }
}


void isort(std::vector<short int>& seq, MPI_Comm comm) {
    int sz = 0, rank = 0;
    MPI_Comm_size(comm, &sz);
    MPI_Comm_rank(comm, &rank);

    int n = seq.size();
    int p = sz;
    int d = n/p;

    std::vector<int> scount(p,d);
    std::vector<int> sdispl(p,d);

    std::sort(std::begin(seq), std::end(seq));

    std::vector<int> splitters(p-1);

    if (rank == sz - 1){
        for (int i=0; i<p-1; i++){
            splitters[i] = seq[(i+1)*d];
        }
    }

    MPI_Bcast(&splitters[0], p-1, MPI_INT, sz-1, comm);

    auto iter = std::begin(seq);
    for (int i=0; i<p-1; ++i){
        auto temp = std::upper_bound(iter, std::end(seq), splitters[i]);
        scount[i] = temp - iter;
        iter = temp;
    }

    scount.back() = std::end(seq) - iter;
    std::partial_sum(std::begin(scount), std::end(scount) - 1, std::begin(sdispl)+1);

    auto rdispl = all_to_all(seq, scount, sdispl, comm);
    multiway_merge(seq, rdispl);
} // isort



#endif // A1_HPP
