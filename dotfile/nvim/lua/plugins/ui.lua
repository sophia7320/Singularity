return {
    {
        "akinsho/bufferline.nvim",
        lazy = false,
        dependencies = {
            "nvim-tree/nvim-web-devicons",
        },
        opts = {},
        keys = {
            { "<leader>bh", ":BufferLineCyclePrev<CR>", silent = true },
            { "<leader>bl", ":BufferLineCycleNext<CR>", silent = true },
            { "<leader>bd", ":bdelete<CR>", silent = true },
            { "<leader>bo", ":BufferLineCloseOthers<CR>", silent = true },
            { "<leader>bp", ":BufferLinePick<CR>", silent = true },
            { "<leader>bc", ":BufferLinePickClose<CR>", silent = true },
        },
    },
    {
        "norcalli/nvim-colorizer.lua",
        event = "BufReadPre",
        opts = {
            "css",
            "html",
            "javascript",
            "lua",
        },
    },
    {
        "lukas-reineke/indent-blankline.nvim",
        main = "ibl",
        event = "VeryLazy",
        config = function()
            local highlight = {
                "IblRed",
                "IblPeach",
                "IblYellow",
                "IblGreen",
                "IblTeal",
                "IblBlue",
                "IblLavender",
            }
            local hooks = require("ibl.hooks")
            hooks.register(hooks.type.HIGHLIGHT_SETUP, function()
                vim.api.nvim_set_hl(0, "IblRed", { fg = "#F38BA8" })
                vim.api.nvim_set_hl(0, "IblPeach", { fg = "#FAB387" })
                vim.api.nvim_set_hl(0, "IblYellow", { fg = "#F9E2AF" })
                vim.api.nvim_set_hl(0, "IblGreen", { fg = "#A6E3A1" })
                vim.api.nvim_set_hl(0, "IblTeal", { fg = "#94E2D5" })
                vim.api.nvim_set_hl(0, "IblBlue", { fg = "#89B4FA" })
                vim.api.nvim_set_hl(0, "IblLavender", { fg = "#B4BEFE" })
            end)
            require("ibl").setup({
                indent = { char = "▎", highlight = highlight },
                scope = { enabled = false },
                exclude = { filetypes = { "dashboard", "alpha", "help", "lazy" } },
            })
        end,
    },
}
