#pragma once

struct Job{
    int jobId;
    int fileId;
    int totalPages;
    int remainingPages;

    Job(int jobId, int fileId, int totalPages, int remainingPages){
        this->jobId = jobId;
        this->fileId = fileId;
        this->totalPages = totalPages;
        this->remainingPages = remainingPages;
    }

    Job(){
        this->jobId = 0;
        this->fileId = 0;
        this->totalPages = 0;
        this->remainingPages = 0;
    }
};