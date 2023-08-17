#include "../../lv_examples.h"
#if LV_USE_TABLE && LV_BUILD_EXAMPLES

static void draw_part_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
    /*If the cells are drawn...*/
    if(dsc->part == LV_PART_ITEMS) {
        uint32_t row = dsc->id /  lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Make the texts in the first cell center aligned*/
        if(row == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*In the first column align the texts to the right*/
        else if(col == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
        }

        /*MAke every 2nd row grayish*/
        if((row != 0 && row % 2) == 0) {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
    }
}


void lv_example_table_1(void)
{
    // Create a table with two rows and two columns
lv_obj_t * table = lv_table_create(lv_scr_act());
lv_table_set_row_cnt(table, 1);
lv_table_set_col_cnt(table, 2);

lv_obj_align(table, LV_ALIGN_CENTER, 0, 0);

// Set the values of the cells
lv_table_set_cell_value(table, 0, 0, "A");
lv_table_set_col_width(table, 0, 5);
lv_table_set_cell_value(table, 0, 1, "B");
lv_table_set_col_width(table, 0, 5);
lv_table_set_cell_value(table, 1, 1, "D");
lv_table_set_col_width(table, 0, 5);

// 设置单元格大小

// lv_table_set_cell_value(table, 1, 0, "C");


// Merge the cells (0,0) and (1,0)
// lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
// lv_table_add_cell_ctrl(table, 1, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

// 合并A和C
lv_table_set_cell_value(table, 0, 0, "A\n\n\nC");


}


#endif
