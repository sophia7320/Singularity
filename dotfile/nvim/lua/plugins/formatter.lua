return {
    {
        "stevearc/conform.nvim",
        event = "BufWritePre",
        cmd = "ConformInfo",

        build = function()
            if vim.fn.executable("astyle") == 0 then
                vim.notify("astyle 未安装，运行 :AstyleInstall 查看安装方法", vim.log.levels.WARN)
            end
        end,

        keys = {
            {
                "<leader>fm",

                mode = { "n", "x" },
                function()
                    require("conform").format({
                        async = true,
                        lsp_format = "fallback",
                    })
                end,
                desc = "[F]ormat buffer",
            },
        },

        opts = {
            formatters = {
                astyle = {
                    args = { "--style=attach", "--keep-one-line-statements" },
                },
            },

            formatters_by_ft = {
                lua = { "stylua" },
                python = { "ruff_format" },
                javascript = { "prettierd", "prettier", stop_after_first = true },
                typescript = { "prettierd", "prettier", stop_after_first = true },
                --go = { "goimports", "gofmt" },
                --rust = { "rustfmt" },
                cpp = { "astyle" },
                c = { "astyle" },
            },
        },
    },
}
