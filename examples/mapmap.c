
#include <stdio.h>
#include <stc/cmap.h>
#include <stc/cstr.h>

declare_cmap_str();
declare_cmap_strkey(cfg, cmap_str, cmap_str_destroy);

int main(void) {
    cmap_cfg config = cmap_init;
    cmap_str init = cmap_init;
    cmap_str_put(&cmap_cfg_insert(&config, "user", init)->value, "name", "Joe");
    cmap_str_put(&cmap_cfg_insert(&config, "user", init)->value, "groups", "proj1,proj3");
    cmap_str_put(&cmap_cfg_insert(&config, "group", init)->value, "proj1", "Energy");
    cmap_str_put(&cmap_cfg_insert(&config, "group", init)->value, "proj2", "Windy");
    cmap_str_put(&cmap_cfg_insert(&config, "group", init)->value, "proj3", "Oil");
    cmap_str_put(&cmap_cfg_insert(&config, "admin", init)->value, "employees", "2302");

    cmap_str_put(&cmap_cfg_insert(&config, "group", init)->value, "proj2", "Wind"); // Update

    c_foreach (i, cmap_cfg, config)
        c_foreach (j, cmap_str, i.item->value)
            printf("%s: %s - %s (%u)\n", i.item->key.str, j.item->key.str, j.item->value.str, i.item->value.bucket_count);

    cmap_cfg_destroy(&config);
}