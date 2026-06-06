return {
    {
        "saghen/blink.cmp",
        version = "1.*",

        dependencies = {
            "rafamadriz/friendly-snippets",
        },

        opts = {
            keymap = {
                preset = "super-tab",
                ["<CR>"] = { "select_and_accept", "fallback" },
                ["<Tab>"] = { "select_and_accept", "fallback" },
            },

            appearance = {
                nerd_font_variant = "mono",
            },

            completion = {
                documentation = { auto_show = true, auto_show_delay_ms = 200, window = { border = "rounded" } },

                accept = { auto_brackets = { enabled = true } },

                ghost_text = { enabled = true },
                trigger = { prefetch_on_insert = false },
            },

            sources = {
                default = { "lsp", "path", "snippets", "buffer" },
                providers = {
                    minuet = {
                        name = "minuet",
                        module = "minuet.blink",
                        score_offset = 8,
                    },
                },
            },

            fuzzy = {
                implementation = "prefer_rust",
            },
        },

        opts_extand = { "sources.default" },
    },
}
