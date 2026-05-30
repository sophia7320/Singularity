return {
    {
        "nvim-treesitter/nvim-treesitter",
        opts = {
            ensure_installed = {
                "lua",
                "vim",
                "vimdoc",
                "query",
                "python",
                "cpp",
                "c",
                "rust",
                "java",
                "markdown",
                "markdown_inline",
                "bash",
                "regex",
                "diff",
            },
        },
    },
    {
        "nvim-treesitter/nvim-treesitter-textobjects",

        branch = "main",

        dependencies = {
            "nvim-treesitter/nvim-treesitter",
        },

        event = "BufReadPost",

        init = function()
            vim.g.no_plugin_maps = true
        end,

        config = function()
            require("nvim-treesitter-textobjects").setup({
                select = {
                    lookahead = true,

                    selection_modes = {
                        ["@function.outer"] = "V",
                        ["@parameter.outer"] = "v",
                    },

                    include_surrounding_whitespace = false,
                },

                move = {
                    set_jumps = true,
                },

                swap = {
                    enable = true,
                },
            })

            local select = require("nvim-treesitter-textobjects.select")

            local kset = vim.keymap.set

            kset({ "x", "o" }, "af", function()
                select.select_textobject("@function.outer", "textobjects")
            end, { desc = "TS: select outer function" })
            kset({ "x", "o" }, "if", function()
                select.select_textobject("@function.inner", "textobjects")
            end, { desc = "T: select inner function " })
            kset({ "x", "o" }, "ac", function()
                select.select_textobject("@class.outer", "textobjects")
            end, { desc = "TS: select outer class" })
            kset({ "x", "o" }, "ic", function()
                select.select_textobject("@class.inne", "textobjects")
            end, { desc = "TS: select inner class" })

            local move = require("nvim-treesitter-textobjects.move")
            kset({ "n", "x", "o" }, "]f", function()
                move.goto_next_start("@function.outer", "textobjects")
            end, { desc = "TS: move to next function start" })
            kset({ "n", "x", "o" }, "[f", function()
                move.goto_previous_start("@function.outer", "textobjects")
            end, { desc = "TS: move to previous function start" })
            kset({ "n", "x", "o" }, "]c", function()
                move.goto_next_start("@class.outer", "textobjects")
            end, { desc = "TS: move to next class start" })
            kset({ "n", "x", "o" }, "[c", function()
                move.goto_previous_start("@class.outer", "textobjects")
            end, { desc = "TS: move to previous class start" })

            local swap = require("nvim-treesitter-textobjects.swap")
            kset("n", "<leader>sn", function()
                swap.swap_next("@parameter.inner", "textobjects")
            end, { desc = "TS: swap next parameter" })
            kset("n", "<leader>sp", function()
                swap.swap_previous("@parameter.inner", "textobjects")
            end, { desc = "TS: swap previous parameter" })
        end,
    },

    {
        "nvim-treesitter/nvim-treesitter-context",

        event = "BufReadPost",

        dependencies = { "nvim-treesitter/nvim-treesitter" },

        opts = {
            enable = true,

            max_lines = 3,

            trim_scope = "outer",

            mode = "cursor",
        },
    },
}
