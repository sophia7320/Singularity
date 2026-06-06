return {
    {
        "nvim-neo-tree/neo-tree.nvim",
        branch = "v3.x",

        dependencies = {
            "nvim-lua/plenary.nvim",
            "nvim-tree/nvim-web-devicons",
            "MunifTanjim/nui.nvim",
        },

        keys = {
            { "<leader>e", "<cmd>Neotree toggle<CR>", desc = "file explorer" },
            { "<leader>E", "<cmd>Neotree reveal<CR>", desc = "pick current file" },
        },
    },
    {
        "folke/which-key.nvim",
        event = "VeryLazy",

        opts = {
            preset = "modern",
            delay = 300,
        },

        keys = {
            {
                "<leader>?",
                function()
                    require("which-key").show({ global = false })
                end,
                desc = "Buffer Local Keymaps {which-key}",
            },
        },
    },
    {
        "nvim-mini/mini.nvim",
        event = "VeryLazy",

        config = function()
            require("mini.pairs").setup({
                modes = { insert = true, command = true, terminal = true },
            })

            require("mini.ai").setup({})
        end,
    },
    {
        "nvim-mini/mini.surround",
        event = "VeryLazy",
        opts = {},
    },
    {
        "nvim-mini/mini.comment",
        event = "VeryLazy",
        opts = {},
    },
}
