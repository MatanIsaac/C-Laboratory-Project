#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdlib.h>
#include "label_table.h"
#include "binary_table.h"

/**
 * @brief Prepares and initiates the second pass of the assembler.
 *
 * Delegates the work to the actual second pass executor. If successful, prints a completion message.
 * Otherwise, logs an error and returns the error status.
 *
 * @param filepath       Path to the source file being assembled.
 * @param binary_table   Pointer to the binary table from the first pass.
 * @param label_table    Pointer to the label table generated in the first pass.
 * @param ICF            Final instruction counter value.
 * @param DCF            Final data counter value.
 * @return VALID_RETURN on success, or INVALID_RETURN if the second pass failed.
 */
int prepare_second_pass(const char* filepath,BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF);

/**
 * @brief Executes the second pass, resolving labels and generating output files (.ob, .ent, .ext).
 *
 * Writes the object file header, resolves unresolved labels in binary nodes, handles entries,
 * and cleans up empty files if no relevant data was written.
 *
 * @param binary_table   Pointer to the binary table generated in the first pass.
 * @param label_table    Pointer to the label table containing all defined labels.
 * @param ICF            Final instruction counter value.
 * @param DCF            Final data counter value.
 * @param filepath       The source file path, used to derive output filenames.
 * @return VALID_RETURN on success, or INVALID_RETURN if any error occurred.
 */
int execute_second_pass(BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF,const char* filepath);

/**
 * @brief Prepares and opens the output files (.ob, .ent, .ext) based on the input file path.
 *
 * Dynamically allocates filenames and initializes file pointers for writing assembler output.
 * Each filename is constructed by modifying the file extension.
 *
 * @param filepath       The path to the source file.
 * @param fp_ob          Pointer to the object file stream pointer.
 * @param fp_ent         Pointer to the entry file stream pointer.
 * @param fp_ext         Pointer to the extern file stream pointer.
 * @param ob_filename    Pointer to store the dynamically allocated .ob filename.
 * @param ent_filename   Pointer to store the dynamically allocated .ent filename.
 * @param ext_filename   Pointer to store the dynamically allocated .ext filename.
 */
void prepare_output_files(const char* filepath, FILE** fp_ob, FILE** fp_ent, FILE** fp_ext,
    char** ob_filename,char** ent_filename,char** ext_filename);

/**
 * @brief Computes and encodes the distance to a label for relative addressing mode.
 *
 * Used to resolve labels in relative addressing format (e.g., `&LABEL`) by calculating
 * the relative jump distance.
 *
 * @param binary_node    The binary node containing the unresolved label.
 * @param node           The label node to resolve against.
 */
void handle_distance_to_label(BinaryNode* binary_node, LabelNode* node);

/**
 * @brief Completes label resolution for all binary nodes with unresolved labels.
 *
 * Resolves each label based on its type (code, data, extern), modifies the wordfield accordingly,
 * and outputs the final result to the object and extern files.
 *
 * @param binary_table   The binary table to finalize.
 * @param label_table    The label table to search for label definitions.
 * @param ob_file        Output file for object code.
 * @param ext_file       Output file for extern labels.
 * @return VALID_RETURN if all labels were resolved successfully, or INVALID_RETURN on failure.
 */
int complete_first_pass(BinaryTable* binary_table,LabelTable* label_table,FILE** ob_file,FILE** ext_file);

/**
 * @brief Writes entries (.entry labels) from the label table into the .ent file.
 *
 * Iterates over the label table and writes labels marked as CODE_ENTRY or DATA_ENTRY
 * to the provided .ent output file.
 *
 * @param label_table    Pointer to the label table.
 * @param ent_file       Pointer to the .ent file stream.
 */
void handle_entries(LabelTable* label_table, FILE** ent_file);

#endif