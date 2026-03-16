#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define MAX_PATH_LEN 256

// 比较两个文本形式的整数文件是否一致
int compare_files(const char* file1, const char* file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        fprintf(stderr, "❌ Error opening file: %s or %s\n", file1, file2);
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 2;
    }

    uint64_t n1, n2;
    int r1, r2;
    size_t index = 0;

    while (1) {
        r1 = fscanf(f1, "%lu", &n1);
        r2 = fscanf(f2, "%lu", &n2);

        if (r1 == EOF && r2 == EOF) {
            fclose(f1);
            fclose(f2);
            return 0; // identical
        }

        if (r1 == EOF || r2 == EOF) {
            printf("❌ Length mismatch at position %zu\n", index);
            fclose(f1);
            fclose(f2);
            return 1;
        }

        if (r1 != 1 || r2 != 1) {
            printf("❌ Error parsing numbers at position %zu\n", index);
            fclose(f1);
            fclose(f2);
            return 1;
        }

        if (n1 != n2) {
            printf("❌ Difference at position %zu: got %lu, expected %lu\n", index, n1, n2);
            fclose(f1);
            fclose(f2);
            return 1;
        }

        index++;
    }
}

// 执行程序并计时，返回是否成功，将耗时写入 *duration_ns（单位：纳秒）
int run_program(const char* program, const char* input_file, const char* output_file, long long* duration_ns) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    char command[3 * MAX_PATH_LEN];
    snprintf(command, sizeof(command), "%s < %s > %s", program, input_file, output_file);

    int ret = system(command);

    clock_gettime(CLOCK_MONOTONIC, &end);

    *duration_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                   (end.tv_nsec - start.tv_nsec);

    if (ret == -1) {
        perror("❌ Failed to execute command");
        return 1;
    } else if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0) {
        printf("❌ Program exited with code %d\n", WEXITSTATUS(ret));
        return 1;
    } else if (!WIFEXITED(ret)) {
        printf("❌ Program terminated abnormally.\n");
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <prefix> <suffix1> [suffix2 ...]\n", argv[0]);
        fprintf(stderr, "Example: %s phr a b c d e\n", argv[0]);
        return 1;
    }

    const char* prefix = argv[1];
    char program_path[MAX_PATH_LEN];
    snprintf(program_path, MAX_PATH_LEN, "../%s/%s", prefix, prefix);

    int total = 0;
    int failed = 0;

    for (int i = 2; i < argc; ++i) {
        total++;
        char suffix = argv[i][0];

        char input_file[MAX_PATH_LEN];
        char my_output_file[MAX_PATH_LEN];
        char correct_output_file[MAX_PATH_LEN];

        snprintf(input_file, MAX_PATH_LEN, "../%s/%s0%c.in", prefix, prefix, suffix);
        snprintf(my_output_file, MAX_PATH_LEN, "../%s/my%s0%c.out", prefix, prefix, suffix);
        snprintf(correct_output_file, MAX_PATH_LEN, "../%s/%s0%c.out", prefix, prefix, suffix);

        printf("🔧 Running test case '%c'...\n", suffix);

        long long duration_ns;

        if (run_program(program_path, input_file, my_output_file, &duration_ns) != 0) {
            printf("❌ Test case '%c': program execution failed.\n\n", suffix);
            failed++;
            continue;
        }

        int cmp_result = compare_files(my_output_file, correct_output_file);
        if (cmp_result == 0) {
            printf("✅ Test case '%c': Passed. (%lld ns ≈ %.3f ms)\n\n", suffix, duration_ns, duration_ns / 1e6);
        } else {
            printf("❌ Test case '%c': Output mismatch. (%lld ns ≈ %.3f ms)\n\n", suffix, duration_ns, duration_ns / 1e6);
            failed++;
        }
    }

    if (failed == 0) {
        printf("🎉 All %d test cases passed successfully!\n", total);
    } else {
        printf("⚠️  %d/%d test cases failed.\n", failed, total);
    }

    return failed;
}
